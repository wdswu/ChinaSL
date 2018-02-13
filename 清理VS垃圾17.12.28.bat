REM 清理VS项目垃圾文件(去掉*.manifest;) By wdswu 2017.12.28
del *.cdf;*.cache;*.obj;*.obj.enc;*.ilk;*.ipdb;*.iobj;*.resources;*.tlb;*.tli;*.tlh /s
del *.tmp;*.rsp;*.pgc;*.pgd;*.meta;*.tlog;*.res;*.pch;*.exp;*.idb;*.rep /s
del *.xdc;*.pdb;*_manifest.rc;*.bsc;*.sbr;*.xml;*.metagen;*.bi;*.ipch;*.log;*.sdf;*.aps /s
del *.lastbuildstate /s

REM 不同项目要更改为不同文件夹

cd .vs\ChinaSL\v15\

attrib +H .suo

del *.* /s /q
rd ipch /s /q

attrib -H
.pause 新项目请配置.vs目录