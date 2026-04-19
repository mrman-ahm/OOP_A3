$f1 = "Q2_Submission.h"
$c1 = Get-Content $f1 -Raw
$c1 = $c1 -replace '(?m)^#include\s+"[^"]+".*\r?\n', ''
Set-Content -Path $f1 -Value $c1 -Encoding UTF8

$f2 = "Q2_Main.cpp"
$c2 = Get-Content $f2 -Raw
$c2 = $c2 -replace '(?m)^#include\s+"[^"]+".*\r?\n', ''
$c2 = "#include `"Q2_Submission.h`"`r`n" + $c2
Set-Content -Path $f2 -Value $c2 -Encoding UTF8
