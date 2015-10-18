@rem -------------------------------
@rem WinRAR打包备份.bat v1.1
@rem zhupf.m2s.venustech 
@rem 2008-9-25 星期四 上午 1:36
@rem -------------------------------

@rem 得到当前目录
@for %%p in ("%cd%") do @set myapp=%%~nxp

@rem 清除已有的bin文件夹
@rd /S/Q bin >nul 2>nul

@rem 创建bin bin\Debug bin\Release 文件夹
@md bin >nul 2>nul
@md bin\Debug >nul 2>nul
@md bin\Release >nul 2>nul

@rem 复制生成的文件到bin文件夹
@copy /Y Debug\%myapp%.exe bin\Debug\ >nul 2>nul
@copy /Y Release\%myapp%.exe bin\Release\ >nul 2>nul
@copy /Y Debug\%myapp%.dll bin\Debug\ >nul 2>nul
@copy /Y Release\%myapp%.dll bin\Release\ >nul 2>nul
@copy /Y Debug\%myapp%.lib bin\Debug\ >nul 2>nul
@copy /Y Release\%myapp%.lib bin\Release\ >nul 2>nul
@copy /Y Debug\%myapp%.ocx bin\Debug\ >nul 2>nul
@copy /Y Release\%myapp%.ocx bin\Release\ >nul 2>nul

@rem 删除Debug和Release文件夹
@rd /S/Q Debug >nul 2>nul
@rd /S/Q Release >nul 2>nul

@rem 创建SRCBAK文件夹
@md SRCBAK >nul 2>nul

@rem 在常见位置查找WinRAR
@FOR %%M IN (C D E F G H I J K L M N O P Q R S T U V W X Y Z) DO (
@set myrar="%%M:\Program Files\WinRAR\rar"
@if exist "%%M:\Program Files\WinRAR\rar.exe" goto findrar)
@echo 找不到WinRAR安装位置,请修改本批处理指定
@echo 请按任意键退出...
@pause >nul 2>nul
@goto Exit
:findrar
@echo 找到的WinRAR安装位置: %myrar%

@rem 用rar压缩需要备份的代码等文件到SRCBAK文件夹内
::rar
@%myrar% a -r -y -x*.rar -x*\cvs -x*\cvs\* -x*\SRCBAK -x*\SRCBAK\* -x*\ref -x*\ref\* -x.cvsignore -x*.log -x*.WW -x*.aps -x*.exp -x*.idb -x*.ilk -x*.ncb -x*.obj -x*.opt -x*.pch -x*.pdb -x*.plg -x*.positions -x*.res -x*.sbr -x*.tlb -x*.tli -x*.tlh -xver.dat -xReadMe.txt -x*.bak -x*.tmp -x*.temp -xThumbs.db -x*.GID  -agMM{月}DD{日}HH{点}MM{分} .\SRCBAK\%myapp%_ >nul 2>nul
::-x*.log -x*.bat
::@rd /S/Q bin >nul 2>nul
@rem pause
:Exit