param (
    [string]$ProjectDir
)

Write-Host "📁 ProjectDir: $ProjectDir"

try {
    $headerPath = Join-Path $ProjectDir "CommandExecution.h"
    Write-Host "🔍 헤더 파일 경로: $headerPath"
    if (-not (Test-Path $headerPath)) {
        Write-Host "❌ 헤더 파일을 찾을 수 없습니다: $headerPath"
        exit 1
    }

    $verLine = Select-String -Path $headerPath -Pattern '#define\s+COMMAND_EXECUTION_VERSION\s+'

    if (-not $verLine) {
        Write-Host "❌ 버전 정보를 찾을 수 없습니다: $headerPath"
        exit 1
    }

    Write-Host "🔎 검색 결과: $($verLine.Count)개 발견"

    if ($verLine.Count -gt 1) {
        Write-Host "⚠️ 경고: 'COMMAND_EXECUTION_VERSION'이 여러 번 발견되었습니다. 중복을 제거해 주세요."
        exit 1
    }

    $pattern = '#define\s+COMMAND_EXECUTION_VERSION\s+"([a-zA-Z0-9._-]+)"'
    $line = $verLine[0].Line
    if ($line -match $pattern) {
        Write-Host "✅ 전체 매칭: $($matches[0])"
        $ver = $matches[1]
        Write-Host "🔢 추출된 버전: $ver"
    } else {
        Write-Host "❌ 버전 정보를 올바르게 추출할 수 없습니다: $line"
        exit 1
    }

    $propsContent = @"
<Project>
    <PropertyGroup>
        <LibraryVersion>$ver</LibraryVersion>
    </PropertyGroup>
</Project>
"@
    Write-Host "
📄 예상 새 파일 내용:
$propsContent
"

    $propsPath = "GeneratedVersion.props"
    if (Test-Path $propsPath) {
        Remove-Item -Path $propsPath -Force
        Write-Host "🗑️ 기존 파일 삭제됨: $propsPath"
    }

    Set-Content -Path $propsPath -Value $propsContent -Encoding UTF8
    Write-Host "✅ $propsPath 파일이 생성되었습니다."
}
catch {
    Write-Host "❌ 오류 발생: $_"
    exit 1
}
