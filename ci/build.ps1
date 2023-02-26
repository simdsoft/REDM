# Install latest cmake version for appveyor ci
# refer to: https://docs.github.com/en/actions/learn-github-actions/environment-variables
if (!($env:GITHUB_ACTIONS -eq "true")) {
    $cmake_ver="3.25.2"
    curl "https://github.com/Kitware/CMake/releases/download/v$cmake_ver/cmake-$cmake_ver-windows-x86_64.zip" -o "cmake-$cmake_ver-windows-x86_64.zip"
    Expand-Archive -Path cmake-$cmake_ver-windows-x86_64.zip -DestinationPath .\
    $cmake_bin = (Resolve-Path .\cmake-$cmake_ver-windows-x86_64\bin).Path
    $env:Path = "$cmake_bin;$env:Path"
}

cmake --version

$T_VS_VERSIONS = @{ '2013' = '12'; '2015' = '14'; '2017' = "15"; '2019' = '16'; '2022' = '17' }

#########  Configure Options
# $env:BUILD_ARCH: 'x86', 'x64'
# $env:VS_VERSION: '2013', '2015', '2017', '2019', '2022'
# $env:TOOLSET: optional, v100
# $env:CXX_STD: optional, 17, 20, 23
$CONFIG_ALL_OPTIONS=@()

$archName = $env:BUILD_ARCH

$vsVerNum = $T_VS_VERSIONS[$env:VS_VERSION]

if ($env:VS_VERSION -ge 2019) {
    if ($archName -eq 'x86') {
        $archName = 'Win32'
    }
    $CONFIG_ALL_OPTIONS += '-G', "Visual Studio $vsVerNum $env:VS_VERSION"
    $CONFIG_ALL_OPTIONS += '-A', "$archName"
} else {
    if ($archName -eq 'x64') {
        $CONFIG_ALL_OPTIONS += '-G', "Visual Studio $vsVerNum $env:VS_VERSION Win64"
    }
}

if ($null -ne $env:TOOLSET) {
    $CONFIG_ALL_OPTIONS += '-T', "$env:TOOLSET"
}

if ($null -ne $env:CXX_STD) {
    $CONFIG_ALL_OPTIONS += "-DCMAKE_CXX_STANDARD=$env:CXX_STD"
}

######### Configure

Write-Output ("CONFIG_ALL_OPTIONS=$CONFIG_ALL_OPTIONS, Count={0}" -f $CONFIG_ALL_OPTIONS.Count)
cmake -B build_$BUILD_ARCH $CONFIG_ALL_OPTIONS

########## Build
cmake --build build_$BUILD_ARCH --config $env:BUILD_TYPE