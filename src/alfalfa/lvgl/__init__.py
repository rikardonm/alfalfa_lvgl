import typing as t

import alfalfa.core
import lobs
import lobs.exporters

_cd = lobs.Path(__file__).parent
_s = _cd / "src"


class Lvgl(
    lobs.Package,
    version=lobs.Version(9, 5, 0),
    tag="lvgl",
):
    """LVGL package"""
    def _prepare_files(self, basepath: lobs.Path):
        _provider = lobs.providers.GitRepoPackage(
            "https://github.com/lvgl/lvgl",
            f"v{self.version}",
        )
        _provider.resolve_to(basepath)

    def _make_project(self, basepath: lobs.Path):
        return lobs.exporters.cmake.CmakeBasedProject()

    def get_exporter_configuration(
        self,
        config_type: type[lobs.exporter.TCFG],
        gen_path: lobs.Path,
    ) -> lobs.exporter.TCFG | None:
        flags: dict[str, t.Any] = {}
        cfg = config_type()
        match cfg:
            case lobs.exporters.esp_idf.EspIdfConfig():
                cfg.config_flags.update(flags)
            case lobs.exporters.cmake.CmakeConfig():
                pass
            case _:
                raise ValueError(f"Unsupported exporter configuration type: {config_type}")
        return cfg


class AlfalfaLvgl(lobs.Package, tag="alfalfa_lvgl"):

    app = lobs.cpp.project.SimpleLibrary(
        artifact_name="alfalfa_lvgl",
        source_files=([
            _s / "display.cpp",
        ]),
        public_includes=[_cd / "inc"],
        linked_libraries=[
            alfalfa.core.Package,
            Lvgl,
        ],
    )
