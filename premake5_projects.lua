local function zyra_join(base_path, rel)
    local base = base_path or ""
    if base == "" then
        return rel
    end
    return path.join(base, rel)
end

function zyra_define_projects(base_path, build_root, int_root)
    local function P(rel) return zyra_join(base_path, rel) end

project "zyra"
    kind "StaticLib"
    targetname "%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}"
    targetdir (build_root)
    objdir    (int_root)
    location (P("zyra_base"))
    
    multiprocessorcompile "On"
    warnings "Extra"
    fatalwarnings { "All" }
    
    filter "configurations:Debug"
        symbols "On"
        defines { "_DEBUG" }
    filter "configurations:Release"
        optimize "On"
        intrinsics "On"
        linktimeoptimization "On"
        defines { "NDEBUG" }
    filter "configurations:Public"
        optimize "On"
        intrinsics "On"
        linktimeoptimization "On"
        defines { "NDEBUG", "ZYRA_PUBLIC" }
    filter {}

    files {
        P("zyra_base/include/**.h"),
        P("zyra_base/include/**.inl"),
        P("zyra_base/src/**.h"),
        P("zyra_base/src/**.cpp")
    }

    includedirs {
        P("zyra_base/include"),
        P("zyra_base/src"),
        P("zyra_base/ext")
    }

    links { P("zyra_base/ext/Detours/detours") }
end