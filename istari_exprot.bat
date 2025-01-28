@echo off
REM Видалення старого файлу, якщо він існує
if exist d:\Develop\Istari\SQL\istari.sql del d:\Develop\Istari\SQL\istari.sql

REM Експортування структури бази даних у новий файл SQL
isql -u SYSDBA -p masterkey -x d:\Develop\Database\istari.gdb -o d:\Develop\Istari\SQL\istari.sql

REM Додавання нового файлу до Git
git add d:\Develop\Istari\SQL\istari.sql
git commit -m "Update database scructure."
git push origin main