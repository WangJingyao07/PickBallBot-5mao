MD       OBJ
COPY     MDK\*.hex   OBJ
COPY     MDK\*.uvoptx   OBJ
COPY     MDK\*.uvprojx  OBJ
COPY     MDK\Objects\*.hex  OBJ
RD /S/Q  MDK

MD       MDK
COPY     OBJ\*.uvoptx   MDK
COPY     OBJ\*.uvprojx  MDK
COPY     OBJ\*.hex  MDK
RD /S/Q  OBJ

exit