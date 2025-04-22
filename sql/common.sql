-- Insertion examples
INSERT INTO Status VALUES (5, 'No RAM', 2, 'Please install some :(');
INSERT INTO Desktops VALUES ('IBM', 'IBM PC 5160 (AT)', 1, 'Intel 286', 'Trident VGA Card', '640 KB', '10 MB', 'N/A', 'N/A', 'VGA', 'MS-DOS 6.22', 0, '');
INSERT INTO Laptops VALUES ('IBM', 'IBM Thinkpad 560E', 1, 'Intel Pentium MMX', 'Trident Cyber9382 1 MB', '80 MB', '4 GB', 'Linksys Etherfast 10/100', 'ESS1688', '800x600', 'N/A', 'Windows 98 FE', 1, '');
INSERT INTO Monitors VALUES ('Envision', 'EN-710e', 1, '1280x1024', 0, '');
INSERT INTO Tablets VALUES ('Acer', 'Aspire 11e', 1, 'Intel Atom', '2.0 GB', '64 GB', 'Windows 8.1', 0, 'I use it for work when I need documentation');
INSERT INTO Phones VALUES ('Samsung', 'Galaxy Camera 2', 1, '1.6 Ghz Exynos Quad Core', '2.0 GB', '64 GB', 'Android 4.3', 0, 'Enjoy using this camera on trips');

-- Read Examples
SELECT Status_Code FROM Status WHERE Priority = 1;
SELECT Model FROM Desktops WHERE OS = 'Debian 11';
SELECT OS FROM Laptops WHERE Model = 'XPS-9570';
SELECT Model FROM Monitors WHERE Display_Resolution = '1600x1200';
SELECT Model FROM Tablets WHERE Status_Code = -1;
SELECT Maker FROM Phones WHERE RAM = '512 MB';

-- Update Examples
UPDATE Status SET Priority = 1 WHERE Status_Code = 3;
UPDATE Desktops SET Storage = '3 TB' WHERE Storage = '1 TB';
UPDATE Laptops SET OS = 'Kubuntu 24.04' WHERE OS = 'Windows 11';
UPDATE Monitors SET Remarks = 'Incredibly large monitor with a high resolution' WHERE Maker = 'Compaq';
UPDATE Tablets SET Storage = '64 GB' WHERE Model = 'iPad 2';
UPDATE Phones SET Remarks = 'hi' WHERE Maker = 'Motorola';

-- Delete Examples
DELETE FROM Status WHERE Status_Code = 2;
DELETE FROM Desktops WHERE OS = 'Debian 12';
DELETE FROM Laptops WHERE Display_Resolution = '1366x768';
DELETE FROM Monitors WHERE Maker = 'Compaq';
DELETE FROM Tablets WHERE Model = 'iPad 2';
DELETE FROM Phones WHERE OS = 'Android 2.2.3';
