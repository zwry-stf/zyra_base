zyra_base = zyra_base or {}

local function join(base, rel)
    if base == nil or base == "" then
        return rel
    end
    return path.join(base, rel)
end

function zyra_base.set_common_project_settings(groups)
    groups = groups or {}
    local debug_groups   = groups.debug   or { "Debug*" }
    local release_groups = groups.release or { "Release*" }
    local public_groups = groups.release or { "Public*" }

    language "C++"
    cppdialect "C++23"
    staticruntime "Off"

    multiprocessorcompile "On"
    warnings "Extra"
    fatalwarnings { "All" }

    for _, cfgpat in ipairs(debug_groups) do
        filter("configurations:" .. cfgpat)
            defines { "_DEBUG" }
    end

    for _, cfgpat in ipairs(release_groups) do
        filter("configurations:" .. cfgpat)
            defines { "NDEBUG" }
    end
    
    for _, cfgpat in ipairs(public_groups) do
        filter("configurations:" .. cfgpat)
            defines { "NDEBUG", "ZYRA_PUBLIC" }
    end

    filter {}
end

function zyra_base.add_projects(opts, groups)
    local base = opts.base or ""
    local build_root = opts.build_root
    local int_root = opts.int_root

    project "zyra_base"
        kind "StaticLib"
        targetname "%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}"
        targetdir (build_root)
        objdir (int_root)
        location (join(base, "zyra_base"))

        zyra_base.set_common_project_settings(groups)

        files {
            join(base, "zyra_base/include/**.h"),
            join(base, "zyra_base/include/**.inl"),
            join(base, "zyra_base/src/**.h"),
            join(base, "zyra_base/src/**.cpp")
        }

        includedirs {
            join(base, "zyra_base/include"),
            join(base, "zyra_base/src"),
            join(base, "zyra_base/ext")
        }

        links { join(base, "zyra_base/ext/Detours/detours") }
end