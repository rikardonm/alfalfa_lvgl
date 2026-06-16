import typing as t

import alfalfa.core
import lobs
import lobs.exporters

_cd = lobs.Path(__file__).parent
_s = _cd / "src"


class LvglOptions:
    refresh_period = lobs.Parameter[float](default=0.033)
    """The desired refresh period for the LVGL library, in seconds.
    The default value is 0.033 seconds, which corresponds to a refresh rate of 30 Hz."""


class Lvgl(
    lobs.Package,
    version=lobs.Version(9, 5, 0),
    tag="lvgl",
):
    options = LvglOptions()

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
        opts = self.options

        if opts.refresh_period.value is not None:
            flags["LV_DEF_REFR_PERIOD"] = int(opts.refresh_period.value * 1000)

        cfg = config_type()
        match cfg:
            case lobs.exporters.esp_idf.EspIdfConfig():
                flags = {f"CONFIG_{k}": v for k, v in flags.items()}
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
