CREATE TABLE "Status" (
	"Status_Code"		INTEGER,
	"Issue"				TEXT,
	"Priority"			INTEGER,
	"Status_Comments"	TEXT,
	PRIMARY KEY("Status_Code")
);

CREATE TABLE "Desktops" (
	"Maker"						TEXT,
	"Model"						TEXT,
	"ID"						INTEGER,
	"CPU"						TEXT,
	"Graphics"					TEXT,
	"RAM"						TEXT,
	"Storage"					TEXT,
	"Network"					TEXT,
	"Audio"						TEXT,
	"Display_Out"				TEXT,
	"OS"						TEXT,
	"Status_Code"				INTEGER,
	"Remarks"					TEXT,
	FOREIGN KEY("Status_Code") 	REFERENCES "Status"("Status_Code")
);

CREATE TABLE "Laptops" (
	"Maker"						TEXT,
	"Model"						TEXT,
	"ID"						INTEGER,
	"CPU"						TEXT,
	"GPU"						TEXT,
	"RAM"						TEXT,
	"Storage"					TEXT,
	"Network"					TEXT,
	"Audio"						TEXT,
	"Display_Resolution"		TEXT,
	"Battery"					TEXT,
	"OS"						TEXT,
	"Status_Code"				INTEGER,
	"Remarks"					TEXT,
	FOREIGN KEY("Status_Code") 	REFERENCES "Status"("Status_Code")
);

CREATE TABLE "Monitors" (
	"Maker"						TEXT,
	"Model"						TEXT,
	"ID"						INTEGER,
	"Display_Resolution"		TEXT,
	"Status_Code"				INTEGER,
	"Remarks"					TEXT,
	FOREIGN KEY("Status_Code") 	REFERENCES "Status"("Status_Code")
);

CREATE TABLE "Tablets" (
	"Maker"						TEXT,
	"Model"						TEXT,
	"ID"						INTEGER,
	"CPU"						TEXT,
	"RAM"						TEXT,
	"Storage"					TEXT,
	"OS"						TEXT,
	"Status_Code"				INTEGER,
	"Remarks"					TEXT,
	FOREIGN KEY("Status_Code") 	REFERENCES "Status"("Status_Code")
);

CREATE TABLE "Phones" (
	"Maker"						TEXT,
	"Model"						TEXT,
	"ID"						INTEGER,
	"CPU"						TEXT,
	"RAM"						TEXT,
	"Storage"					TEXT,
	"OS"						TEXT,
	"Status_Code"				INTEGER,
	"Remarks"					TEXT,
	FOREIGN KEY("Status_Code") 	REFERENCES "Status"("Status_Code")
);

INSERT INTO Status VALUES (0, 'Device works properly', 1000, 'No need to do anything, the device is functioning as it should');
INSERT INTO Status VALUES (1, 'Missing/Dead Battery', 100, 'Computer should work, however do not expect the device to run without it plugged in');
INSERT INTO Status VALUES (2, 'Missing/Dead Hard Drive', 20, 'Device needs a new hard drive. Trivial for SATA buses, but for IDE I would suggest Compact Flash, Disk-on-modules, or other alternative means');
INSERT INTO Status VALUES (3, 'Bad Capacitors', 3, 'Device cannot run, or bulging capacitors were spotted. While it may run, treat it with extreme caution.');
INSERT INTO Status VALUES (4, 'Dead CMOS Battery', 500, 'Will still run, but it will mean you need to input the date and time every single time (or on older computers, hard drive and floppy information).');
INSERT INTO Status VALUES (-1, 'Parts Only', 10000, 'Computer is presumed to be irreparable and has been used as parts.');

INSERT INTO Desktops VALUES ('Apple', 'iMac G4', 1, 'PowerPC G4 1.0 GHz', 'Geforce4 MX', '1.0 GB DDR', '60 GB', '10/100 Ethernet', 'Apple Pro Speakers', 'Mini-VGA', 'OS X 10.4.11', 4, '');
INSERT INTO Desktops VALUES ('Dell', 'Optiplex 755', 1, 'Intel Core 2 Duo E7300', 'Integrated Graphics', '2.0 GB DDR2', '6 TB', '10/100 Ethernet', 'N/A', 'VGA', 'Debian 11', 0, 'A computer I use as a storage server.');
INSERT INTO Desktops VALUES ('Lenovo', 'OH50-55', 1, 'AMD A8-7600', 'Integrated Graphics', '6.0 GB DDR3', '1 TB', '10/100 Ethernet', 'N/A', 'DVI, HDMI', 'Debian 12', 0, 'Minecraft Server.');

INSERT INTO Laptops VALUES ('Dell', 'Latitude E5430', 1, 'Intel i3-3100M', 'Integrated Graphics', '8.0 GB DDR3', '200 GB', 'Broadcom NetXtreme BCM5761', 'Intel HDA', '1366x768', 'SANYO Dell R2D9M67', 'Debian 12', 0, 'My college laptop');
INSERT INTO Laptops VALUES ('Dell', 'XPS 9570', 1, 'Intel i5-8300', 'Geforce GTX 1050 Ti Mobile', '8.0 GB DDR4', '256 GB', 'Killer Wireless 1535', 'Intel HDA', '3840x2160', 'Dell OEM', 'Windows 11', 0, 'Found it for only $100');

INSERT INTO Monitors VALUES ('Compaq', 'P1610', 1, '1920x1200', 0, 'This CRT monitor has one of the greatest resolutions I have ever seen on a CRT');
INSERT INTO Monitors VALUES ('Apple', '21" Studio Display', 1, '1600x1200', 0, 'Absolute behemoth');

INSERT INTO Tablets VALUES ('Apple', 'iPad 2', 1, 'Apple A5', '512 MB', '16 GB', 'iOS 9.3.5', -1, 'It is bricked sadly');

INSERT INTO Phones VALUES ('Motorola', 'Droid', 1, 'ARM Cortex-A8 550 MHz', '256 MB', '512 MB', 'Android 2.2.3', 0, 'I wanted this phone when I was a kid');
