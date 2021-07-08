workspace "ShaderPlayground"
  architecture "x64"
  configurations { "Debug", "Release" }

  outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    
  ExternalDirectories = {}
  ExternalDirectories["Glad"]  = "build/ThirdParty/Glad/include"
  ExternalDirectories["GLFW"]  = "build/ThirdParty/GLFW/include"

  include "build/ThirdParty/Glad"
  include "build/ThirdParty/GLFW"

project "ShaderPlayground"
  location "build"
  kind "ConsoleApp"
  language "C++"

  targetdir ( "bin/".. outputdir .. "/%{prj.name}" )
  objdir    ( "bin/intermediates/" .. outputdir .. "/%{prj.name}" )

  files { "build/src/**.cpp", "build/include/**.h" }
  includedirs { "build/src", "build/include", "%{ExternalDirectories.Glad}", "%{ExternalDirectories.GLFW}" }
  links { "Glad", "GLFW", "opengl32.lib" }

  pchheader "sppch.h"
  pchsource "build/src/sppch.cpp"

  filter "system:windows" 
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"
  
    defines { "SP_WIN" }

  filter { "configurations:Debug" }
    defines { "DEBUG" }
    symbols "On"

  filter { "configurations:Release" }
    defines { "NDEBUG" }
    optimize "On"

