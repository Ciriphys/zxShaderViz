project "Glad"
    staticruntime "on"
    kind "StaticLib"
    language "C"

    targetdir ( "bin/".. outputdir .. "/%{prj.name}" )
    objdir    ( "bin/intermediates/" .. outputdir .. "/%{prj.name}" )

    files { "include/glad/glad.h", "include/KHR/khrplatform", "src/glad.c" }
    includedirs { "include" }

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"