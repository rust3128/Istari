@echo off
REM ��������� ������� �����, ���� �� ����
if exist d:\Develop\Istari\SQL\istari.sql del d:\Develop\Istari\SQL\istari.sql

REM ������������� ��������� ���� ����� � ����� ���� SQL
isql -u SYSDBA -p masterkey -x d:\Develop\Database\istari.gdb -o d:\Develop\Istari\SQL\istari.sql

REM ��������� ������ ����� �� Git
git add d:\Develop\Istari\SQL\istari.sql
git commit -m "Update database scructure."
git push origin main