workspace "ShaderPlayground"
  architecture "x64"
  configurations { "Debug", "Release", "Distribution" }

  outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    
  ExtLibs = {}
  ExtLibs["Glad"]  = "build/ThirdParty/Glad/include"
  ExtLibs["GLFW"]  = "build/ThirdParty/GLFW/include"
  ExtLibs["Yaml"]  = "build/ThirdParty/YAML/include"
  ExtLibs["ImGui"] = "build/ThirdParty/ImGui"

  IncludeDirectories = {}
  IncludeDirectories["glm"] = "build/ThirdParty/glm"

  include "build/ThirdParty/Glad"
  include "build/ThirdParty/GLFW"
  include "build/ThirdParty/ImGui"
  include "build/ThirdParty/YAML"

project "ShaderPlayground"
  location "build"
  kind "ConsoleApp"
  language "C++"

  targetdir ( "bin/".. outputdir .. "/%{prj.name}" )
  objdir    ( "bin/intermediates/" .. outputdir .. "/%{prj.name}" )

  files
  { 
    "build/src/**.cpp", 
    "build/include/**.h",
    "%{IncludeDirectories.glm}/glm/**.hpp",
  }

  includedirs 
  {
     "build/src", 
     "build/include",
     "%{IncludeDirectories.glm}",
     "%{ExtLibs.Glad}", 
     "%{ExtLibs.GLFW}", 
     "%{ExtLibs.ImGui}",
     "%{ExtLibs.Yaml}"
  }

  links { "Glad", "GLFW", "ImGui", "Yaml-cpp", "opengl32.lib" }

  pchheader "sppch.h"
  pchsource "build/src/sppch.cpp"

  filter "system:windows" 
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"
    system "windows"

    defines { "SP_WIN" }

  filter { "configurations:Debug" }
    defines { "SP_DEBUG", "DEBUG" }
    symbols "On"

  filter { "configurations:Release" }
    defines { "SP_RELEASE", "NDEBUG" }
    optimize "On"
    linkoptions { "/SUBSYSTEM:WINDOWS" }

  filter { "configurations:Distribution" }
    defines { "SP_DIST", "SP_HIDE_LOGS" }
    optimize "On"
    linkoptions { "/SUBSYSTEM:WINDOWS" }
