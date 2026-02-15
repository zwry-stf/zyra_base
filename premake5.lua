workspace "zyra"
    configurations { "Debug", "Release", "Public" }
    platforms { "x86", "x64" }
    language "C++"
    cppdialect "C++23"
    staticruntime "Off"

    filter "platforms:x86"
        architecture "x86"
    filter "platforms:x64"
        architecture "x86_64"
    filter {}
    
    local build_root = "build/%{prj.name}"
    local int_root   = "build/%{prj.name}/%{cfg.buildcfg}/%{cfg.platform}"

    local action = _ACTION
    if action == nil or action == "" then
        action = "unknown_action"
    end
    local host = os.host() or "unknown_host"
        
    location ("out/" .. action .. "/" .. host)
    
include "premake/zyra_base.lua"

zyra_base.add_projects(
    {
        base = "",
        build_root = build_root,
        int_root = int_root,
    }
)