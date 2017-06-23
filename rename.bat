@echo off

for /r %%i in (*.hpp) do (
 echo ren %%~fi %%~di%%~pi%%~ni.inl
 ren %%~fi *.inl
)

@echo on