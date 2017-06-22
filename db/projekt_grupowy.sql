DROP TABLE IF EXISTS "Descriptions";
CREATE TABLE Descriptions (
  id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  images_id   INTEGER,
  description TEXT,
  FOREIGN KEY (Images_id) REFERENCES Images (id)
);
DROP TABLE IF EXISTS "ImageInTable";
CREATE TABLE ImageInTable (
  id        INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  Tables_id INTEGER,
  Images_id INTEGER,
  xPosition INTEGER,
  yPosition INTEGER,
  FOREIGN KEY (Tables_id) REFERENCES Tables (id),
  FOREIGN KEY (Images_id) REFERENCES Images (id)
);
DROP TABLE IF EXISTS "Images";
CREATE TABLE "Images" (
  "id"         INTEGER PRIMARY KEY  AUTOINCREMENT NOT NULL,
  "path"       TEXT,
  "popularity" INTEGER
);
DROP TABLE IF EXISTS "Tables";
CREATE TABLE "Tables" (
  "id"         INTEGER PRIMARY KEY  AUTOINCREMENT NOT NULL,
  "name"       INTEGER,
  "rowsNumber" INTEGER                            NOT NULL,
  "colsNumber" INTEGER                            NOT NULL,
  "lastUpdate" DATETIME DEFAULT CURRENT_TIMESTAMP
);
