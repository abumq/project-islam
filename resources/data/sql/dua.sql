-- Dua table
--     ID
--     Arabic
--     English
--     Translation
--     Source 1 = Quran, 2 = Hadith
--     LocationInSource
DROP TABLE IF EXISTS Dua;
CREATE TABLE IF NOT EXISTS Dua (ID INTEGER PRIMARY KEY ASC, Arabic STRING NOT NULL, English STRING NOT NULL, Translation STRING NOT NULL, Source INTEGER NOT NULL, LocationInSource STRING NOT NULL);

INSERT INTO Dua (Arabic, English, Translation, Source, LocationInSource) VALUES ('رَبِّ ابْنِ لِي عِنْدَكَ بَيْتًا فِي الْجَنَّةِ وَنَجِّنِي مِنْ فِرْعَوْنَ وَعَمَلِهِ وَنَجِّنِي مِنَ الْقَوْمِ الظَّالِمِينَ', 'Rabbiibni lee AAindaka baytan fee aljannati wanajjinee min firAAawnawaAAamalihi wanajjinee mina alqawmi aththalimeen', 'O my Lord! Build for me, in nearness to Thee, a mansion in the Garden, and save me from Pharaoh and his doings, and save me from those that do wrong', 1, '66:11');
INSERT INTO Dua (Arabic, English, Translation, Source, LocationInSource) VALUES ('اَللّٰهُمَّ حَاسِبْنِىْ حِسَا بًا يَّسِيْرًا', 'Allahumma hasibni hisabain yaseera', 'O Allaah make reckoning easy for me', 1, '84:8');
