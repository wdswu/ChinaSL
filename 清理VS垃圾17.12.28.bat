REM ����VS��Ŀ�����ļ�(ȥ��*.manifest;) By wdswu 2017.12.28
del *.cdf;*.cache;*.obj;*.obj.enc;*.ilk;*.ipdb;*.iobj;*.resources;*.tlb;*.tli;*.tlh /s
del *.tmp;*.rsp;*.pgc;*.pgd;*.meta;*.tlog;*.res;*.pch;*.exp;*.idb;*.rep /s
del *.xdc;*.pdb;*_manifest.rc;*.bsc;*.sbr;*.xml;*.metagen;*.bi;*.ipch;*.log;*.sdf;*.aps /s
del *.lastbuildstate /s

REM ��ͬ��ĿҪ����Ϊ��ͬ�ļ���

cd .vs\ChinaSL\v15\

attrib +H .suo

del *.* /s /q
rd ipch /s /q

attrib -H
.pause ����Ŀ������.vsĿ¼