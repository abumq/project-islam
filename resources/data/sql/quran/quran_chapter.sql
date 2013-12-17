-- QuranChapter table
--     ID
--     RevelationOrder
--     Classification     :    1 = Meccan, 2 = Medinan
--     ArabicScriptName
--     ArabicName
--     EnglishName
--     Pronounciation
--     RukuhCount
--     VersesCount
--     SajdahCount
--     Position           :    Position in Quran seperated by comma
--     Muqattaat          :    Muqattaat if any otherwise null
DROP TABLE IF EXISTS QuranChapter;
CREATE TABLE IF NOT EXISTS QuranChapter (ID INTEGER PRIMARY KEY ASC, RevelationOrder INTEGER NOT NULL, Classification INTEGER NOT NULL, ArabicScriptName STRING NOT NULL, ArabicName STRING NOT NULL, EnglishName STRING NOT NULL, Pronounciation STRING NOT NULL, RukuhCount INTEGER NOT NULL, VersesCount INTEGER NOT NULL, SajdahCount INTEGER NOT NULL, Position STRING NOT NULL, Muqattaat STRING);

-- Chapter. 1
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount, SajdahCount, Position, Muqattaat) VALUES(5, 1, 'الفاتحة', 'Al-Fatihah', 'The Opener', 'Al-Fātiĥah', 1, 7, 0, '1', NULL);

-- Chapter. 2
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount, SajdahCount, Position, Muqattaat) VALUES(87, 2, 'البقرة', 'Al-Baqarah', 'The Cow', 'Al-Baqarah', 40, 286, 0, '1,2,3', 'Alif Lām Mīm');

-- Chapter. 3
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount, SajdahCount, Position, Muqattaat) VALUES(89, 2, 'آل عمران', 'Al-Imran', 'The Family Of Imran', 'Āli `Imrān', 20, 200, 0, '3,4', 'Alif Lām Mīm');

-- Chapter. 4
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount, SajdahCount, Position, Muqattaat) VALUES(92, 2, 'النساء', 'An-Nisa', 'Women', 'An-Nisā', 24, 176, 0, '4,5,6', NULL);

-- Chapter. 5
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount, SajdahCount, Position, Muqattaat) VALUES(112, 2, 'المائدة', 'Al-Maidah', 'The Table Spread with Food', 'Al-Mā`idah', 16, 120, 0, '6,7', NULL);

-- Chapter. 6
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(55, 1, 'الأنعام', 'Al-Aanaam', 'The Cattle', 'Al-`An`ām', 20, 165, 0, '7,8', NULL);

-- Chapter. 7
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(39, 1, 'الأعراف', 'Al-Aaraaf', 'The Heights', 'Al-A`rāf', 24, 206, 1, '8,9', 'Alif Lām Mīm Ṣād');

-- Chapter. 8
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(88, 2, 'الأنفال', 'Al-Anfaal', 'The Spoils of War', 'Al-`Anfāl', 10, 75, 0, '9,10', NULL);

-- Chapter. 9
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(113, 2, 'التوبة', 'At-Tawba', 'The Repentance', 'At-Tawbah', 16, 129, 0, '10,11', NULL);

-- Chapter. 10
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(51, 1, 'يونس', 'Yunus', 'Jonah', 'Yūnus', 11, 109, 0, '11', 'Alif Lām Rā');

-- Chapter. 11
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(52, 1, 'هود', 'Hud', 'Jonah', 'Hūd', 10, 123, 0, '11,12', 'Alif Lām Rā');

-- Chapter. 12
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(53, 1, 'يوسف', 'Yusuf', 'Joseph', 'Yūsuf', 12, 111, 0, '12,13', 'Alif Lām Rā');

-- Chapter. 13
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(96, 1, 'الرعد', 'Ar-Raad', 'The Thunder', 'Ar-Ra`d', 6, 43, 1, '13', 'Alif Lām Mīm Rā');

-- Chapter. 14
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(72, 1, 'ابراهيم', 'Ibrahim', 'Abrahim', 'Ibrāhīm', 7, 52, 0, '13', 'Alif Lām Rā');

-- Chapter. 15
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(54, 1, 'الحجر', 'Al-Hijr', 'The Rocky Tract', 'Al-Ĥijr', 6, 99, 0, '14', 'Alif Lām Rā');

-- Chapter. 16
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(70, 1, 'النحل', 'An-Nahl', 'The Bee', 'An-Naĥl', 16, 128, 1, '14', NULL);

-- Chapter. 17
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(50, 1, 'الإسراء', 'Al-Isra', 'The Night Journey', 'Al-`Isrā`', 12, 111, 1, '15', NULL);

-- Chapter. 18
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(69, 1, ' الكهف', 'Al-Kahf', 'The Cave', 'Al-Kahf', 12, 110, 0, '15,16', NULL);

-- Chapter. 19
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(44, 1, 'مريم', 'Maryam', 'Mary', 'Maryam', 6, 98, 1, '16', 'Kāf Hāʾ Yāʾ ʿAin Ṣād');

-- Chapter. 20
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(45, 1, 'طه', 'Ta-Ha', 'Ta-Ha', 'Ţāhā', 8, 135, 0, '16', 'Ṭāʾ Hāʾ');

-- Chapter. 21
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(73, 1, 'الأنبياء', 'Al-Anbiya', 'The Prophets', 'Al-`Anbyā`', 7, 112, 0, '17', NULL);

-- Chapter. 22
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(103, 2, 'الحج', 'Al-Hajj', 'The Pilgrimage', 'Al-Ĥaj', 10, 78, 2, '17', NULL);

-- Chapter. 23
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(74, 1, 'المؤمنون', 'Al-Muminoon', 'The Believers', 'Al-Mu`minūn', 6, 118, 0, '18', NULL);

-- Chapter. 24
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(102, 2, 'النور', 'An-Nur', 'The Light', 'An-Nūr', 9, 64, 0, '18', NULL);

-- Chapter. 25
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(42, 1, 'الفرقان', 'Al-Furqan', 'The Criterian', 'Al-Furqān', 6, 77, 1, '18,19', NULL);

-- Chapter. 26
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(47, 1, 'الشعراء', 'Ash-Shuaara', 'The Poets', 'Ash-Shu`arā`', 11, 227, 0, '19', 'Ṭāʾ Sīn Mīm');

-- Chapter. 27
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(48, 1, 'النمل', 'An-Naml', 'The Ant', 'An-Naml', 7, 93, 1, '19,20', 'Ṭāʾ Sīn');

-- Chapter. 28
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(49, 1, 'القصص', 'Al-Qasas', 'The Stories', 'Al-Qaşaş', 9, 88, 0, '20', 'Ṭāʾ Sīn Mīm');

-- Chapter. 29
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(85, 2, 'العنكبوت', 'Al-Ankabut', 'The Spider', 'Al-`Ankabūt', 7, 69, 0, '20', 'ʾAlif Lām Mīm');

-- Chapter. 30
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(84, 2, 'الروم', 'Ar-Rum', 'The Romans', 'Ar-Rūm', 6, 60, 0, '21', 'ʾAlif Lām Mīm');

-- Chapter. 31
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(57, 1, ' لقمان', 'Luqman', 'Luqman', 'Luqmān', 4, 34, 0, '21', 'ʾAlif Lām Mīm');

-- Chapter. 32
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(75, 1, 'السجدة', 'As-Sajda', 'The Prostration', 'As-Sajdah', 3, 30, 1, '21', 'ʾAlif Lām Mīm');

-- Chapter. 33
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(90, 2, 'الأحزاب', 'Al-Ahzab', 'The Combined Forces', 'Al-`Aĥzāb', 9, 73, 0, '21,22', NULL);

-- Chapter. 34
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(58, 1, 'سبإ', 'Saba', 'Sheba', 'Saba`', 6, 54, 0, '22', NULL);

-- Chapter. 35
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(43, 1, 'فاطر', 'Fatir', 'Originator', 'Fāţir', 5, 45, 0, '21,22', NULL);

-- Chapter. 36
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(41, 1, 'يس', 'Ya Sin', 'Ya Sin', 'Yā-Sīn', 5, 83, 0, '22,23', 'Yāʾ Sīn');

-- Chapter. 37
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(56, 1, 'الصافات', 'As-Saaffat', 'Those who set the Ranks', 'Aş-Şāffāt', 5, 182, 0, '23', NULL);

-- Chapter. 38
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(38, 1, 'ص', 'Sad', 'The Letter "Saad"', 'Şād', 5, 88, 1, '23', 'Ṣād');

-- Chapter. 39
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(59, 1, 'الزمر', 'Az-Zumar', 'The Troops', 'Az-Zumar', 8, 75, 0, '23,24', NULL);

-- Chapter. 40
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(60, 1, 'غافر', 'Ghafir', 'The Forgiver', 'Ghāfir', 9, 85, 0, '24', 'Ḥāʾ Mīm');

-- Chapter. 41
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(61, 1, 'فصلت', 'Fussilat', 'Explained in Detail', 'Fuşşilat', 6, 54, 1, '24,25', 'Ḥāʾ Mīm');

-- Chapter. 42
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(62, 1, 'الشورى', 'Ash-Shura', 'The Consultation', 'Ash-Shūraá', 5, 53, 0, '25', 'Ḥāʾ Mīm; ʿAin Sīn Qāf');

-- Chapter. 43
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(63, 1, 'الزخرف', 'Az-Zukhruf', 'The Ornaments of Gold', 'Az-Zukhruf', 7, 89, 0, '25', 'Ḥāʾ Mīm');

-- Chapter. 44
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(64, 1, 'الدخان', 'Ad-Dukhan', 'The Smoke', 'Ad-Dukhān', 3, 59, 0, '25', 'Ḥāʾ Mīm');

-- Chapter. 45
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(65, 1, 'الجاثية', 'Al-Jathiya', 'The Crouching', 'Al-Jāthiyah', 4, 37, 0, '25', 'Ḥāʾ Mīm');

-- Chapter. 46
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(66, 1, 'الأحقاف', 'Al-Ahqaf', 'The Wind-Curved Sandhills', 'Al-`Aĥqāf', 4, 35, 0, '26', 'Ḥāʾ Mīm');

-- Chapter. 47
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(95, 2, 'محمد', 'Muhammad', 'Muhammad', 'Muĥammad', 4, 38, 0, '26', NULL);

-- Chapter. 48
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(111, 2, 'الفتح', 'Al-Fath', 'The Victory', 'Al-Fatĥ', 4, 29, 0, '26', NULL);

-- Chapter. 49
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(106, 2, 'الحجرات', 'Al-Hujurat', 'The Rooms', 'Al-Ĥujurāt', 2, 18, 0, '26', NULL);

-- Chapter. 50
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(34, 1, 'ق', 'Qaf', 'The Letter "Qaf"', 'Qāf', 3, 45, 0, '26', 'Qāf');

-- Chapter. 51
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(67, 1, 'الذاريات', 'Adh-Dhariyat', 'The Winnowing Winds', 'Adh-Dhāriyāt', 3, 60, 0, '26,27', NULL);

-- Chapter. 52
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(76, 1, 'الطور', 'At-Tur', 'The Mount', 'Aţ-Ţūr', 2, 49, 0, '27', NULL);

-- Chapter. 53
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(23, 1, ' النجم', 'An-Najm', 'The Star', 'An-Najm', 3, 62, 1, '27', NULL);

-- Chapter. 54
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(37, 1, 'القمر', 'Al-Qamar', 'The Moon', 'Al-Qamar', 3, 55, 0, '27', NULL);

-- Chapter. 55
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(97, 2, 'الرحمن', 'Ar-Rahman', 'The Beneficent', 'Ar-Raĥmān', 3, 78, 0, '27', NULL);

-- Chapter. 56
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(46, 1, ' الواقعة', 'Al-Waqia', 'The Inevitable', 'Al-Wāqi`ah', 3, 96, 0, '27', NULL);

-- Chapter. 57
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(94, 2, 'الحديد', 'Al-Hadid', 'The Iron', 'Al-Ĥadīd', 4, 29, 0, '27', NULL);

-- Chapter. 58
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(105, 2, 'المجادلة', 'Al-Mujadila', 'The Pleading Woman', 'Al-Mujādila', 3, 22, 0, '28', NULL);

-- Chapter. 59
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(101, 2, 'الحشر', 'Al-Hashr', 'The Exile', 'Al-Ĥashr', 3, 24, 0, '28', NULL);

-- Chapter. 60
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(91, 2, ' الممتحنة', 'Al-Mumtahina', 'She that is to be examined', 'Al-Mumtaĥanah', 2, 13, 0, '28', NULL);

-- Chapter. 61
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(109, 2, 'الصف', 'As-Saff', 'The Ranks.', 'Aş-Şaf', 2, 14, 0, '28', NULL);

-- Chapter. 62
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(110, 2, 'الجمعة', 'Al-Jumua', 'The Friday', 'Al-Jumu`ah', 2, 11, 0, '28', NULL);

-- Chapter. 63
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(104, 2, 'المنافقون', 'Al-Munafiqun', 'The Hypocrites', 'Al-Munāfiqūn', 2, 11, 0, '28', NULL);

-- Chapter. 64
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(108, 2, 'التغابن', 'At-Taghabun', 'The Mutual Disillusion', 'At-Taghābun', 2, 18, 0, '28', NULL);

-- Chapter. 65
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(99, 2, 'الطلاق', 'At-Talaq', 'The Divorce', 'Aţ-Ţalāq', 2, 12, 0, '28', NULL);

-- Chapter. 66
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(107, 2, 'التحريم', 'At-Tahrim', 'The Prohibtiion', 'At-Taĥrīm', 2, 12, 0, '28', NULL);

-- Chapter. 67
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(77, 1, 'الملك', 'Al-Mulk', 'Al-Mulk', 'The Sovereignty', 2, 30, 0, '29', NULL);

-- Chapter. 68
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(2, 1, 'القلم', 'Al-Qalam', 'The Pen', 'Al-Qalam', 2, 52, 0, '29', 'Nūn');

-- Chapter. 69
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(78, 1, 'الحاقة', 'Al-Haaqqa', 'The Reality', 'Al-Ĥāqqah', 2, 52, 0, '29', NULL);

-- Chapter. 70
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(79, 1, 'المعارج', 'Al-Maarij', 'The Ascending Stairways', 'Al-Ma`ārij', 2, 44, 0, '29', NULL);

-- Chapter. 71
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(71, 1, 'نوح', 'Nuh', 'Noah', 'Nūĥ', 2, 28, 0, '29', NULL);

-- Chapter. 72
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(40, 1, 'الجن', 'Al-Jinn', 'The Jinn', 'Al-Jinn', 2, 28, 0, '29', NULL);

-- Chapter. 73
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(3, 1, 'المزمل', 'Al-Muzzammil', 'The Enshrouded One', 'Al-Muzzammil', 2, 20, 0, '29', NULL);

-- Chapter. 74
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(4, 1, 'المدثر', 'Al-Muddathir', 'The Cloaked One', 'Al-Muddaththir', 2, 56, 0, '29', NULL);

-- Chapter. 75
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(31, 1, 'القيامة', 'Al-Qiyama', 'The Resurrection', 'Al-Qiyāmah', 2, 40, 0, '29', NULL);

-- Chapter. 76
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(98, 2, 'الانسان', 'Al-Insan', 'The Man', 'Al-`Insān', 2, 31, 0, '29', NULL);

-- Chapter. 77
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(33, 1, 'المرسلات', 'Al-Mursalat', 'The Emissaries', 'Al-Mursalāt', 2, 50, 0, '29', NULL);

-- Chapter. 78
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(80, 1, 'النبإ', 'An-Naba', 'The Tidings', 'An-Naba`', 2, 40, 0, '30', NULL);

-- Chapter. 79
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(81, 1, 'النازعات', 'An-Naziat', 'Those who drag forth', 'An-Nāzi`āt', 2, 46, 0, '30', NULL);

-- Chapter. 80
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(24, 1, 'عبس', 'Abasa', 'He Frowned', '`Abasa', 1, 42, 0, '30', NULL);

-- Chapter. 81
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(7, 1, 'التكوير', 'At-Takwir', 'The Overthrowing', 'At-Takwīr', 1, 29, 0, '30', NULL);

-- Chapter. 82
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(82, 1, 'الإنفطار', 'Al-Infitar', 'The Cleaving', 'Al-`Infiţār', 1, 19, 0, '30', NULL);

-- Chapter. 83
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(86, 2, 'المطففين', 'Al-Mutaffifin', 'The Defrauding', 'Al-Muţaffifīn', 1, 36, 0, '30', NULL);

-- Chapter. 84
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(83, 1, 'الإنشقاق', 'Al-Inshiqaq', 'The Sundering', 'Al-`Inshiqāq', 1, 25, 1, '30', NULL);

-- Chapter. 85
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(27, 1, 'البروج', 'Al-Burooj', 'The Mansions of the Stars', 'Al-Burūj', 1, 22, 0, '30', NULL);

-- Chapter. 86
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(36, 1, 'الطارق', 'At-Tariq', 'The Nightcommer', 'Aţ-Ţāriq', 1, 17, 0, '30', NULL);

-- Chapter. 87
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(8, 1, 'الأعلى', 'Al-Ala', 'The Most High', 'Al-`A`lá', 1, 19, 0, '30', NULL);

-- Chapter. 88
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(68, 1, 'الغاشية', 'Al-Ghashiya', 'The Overwhelming', 'Al-Ghāshiyah', 1, 26, 0, '30', NULL);

-- Chapter. 89
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(10, 1, 'الفجر', 'Al-Fajr', 'The Dawn', 'Al-Fajr', 1, 30, 0, '30', NULL);

-- Chapter. 90
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(35, 1, 'البلد', 'Al-Balad', 'The City', 'Al-Balad', 1, 20, 0, '30', NULL);

-- Chapter. 91
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(26, 1, ' الشمس', 'Ash-Shams', 'The Sun', 'Ash-Shams', 1, 15, 0, '30', NULL);

-- Chapter. 92
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(9, 1, 'الليل', 'Al-Lail', 'The Night', 'Al-Layl', 1, 21, 0, '30', NULL);

-- Chapter. 93
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(11, 1, 'الضحى', 'Ad-Dhuha', 'The Morning Hours', 'Ađ-Đuĥaá', 1, 11, 0, '30', NULL);

-- Chapter. 94
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(12, 1, 'الشرح', 'Al-Inshirah', 'The Relief', 'Ash-Sharĥ', 1, 8, 0, '30', NULL);

-- Chapter. 95
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(28, 1, 'التين', 'At-Tin', 'The Fig', 'At-Tīn', 1, 8, 0, '30', NULL);

-- Chapter. 96
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(1, 1, 'العلق', 'Al-Alaq', 'The Clot', 'Al-`Alaq', 1, 19, 1, '30', NULL);

-- Chapter. 97
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(25, 1, 'القدر', 'Al-Qadr', 'The Power', 'Al-Qadr', 1, 5, 0, '30', NULL);

-- Chapter. 98
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(100, 2, 'البينة', 'Al-Bayyina', 'The Clear Proof', 'Al-Bayyinah', 1, 8, 0, '30', NULL);

-- Chapter. 99
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(93, 2, 'الزلزلة', 'Az-Zalzala', 'The Earthquake', 'Az-Zalzalah', 1, 8, 0, '30', NULL);

-- Chapter. 100
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(14, 1, 'العاديات', 'Al-Adiyat', 'The Courser', 'Al-`Ādiyāt', 1, 11, 0, '30', NULL);

-- Chapter. 101
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(30, 1, 'القارعة', 'Al-Qaria', 'The Calamity', 'Al-Qāri`ah', 1, 11, 0, '30', NULL);

-- Chapter. 102
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(16, 1, ' التكاثر', 'At-Takathur', 'The Rivalry in world increase', 'At-Takāthur', 1, 8, 0, '30', NULL);

-- Chapter. 103
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(13, 1, ' العصر', 'Al-Asr', 'The Declining Day', 'Al-`Asr', 1, 3, 0, '30', NULL);

-- Chapter. 104
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(32, 1, ' الهمزة', 'Al-Humazah', 'The Traducer', 'Al-Humazah', 1, 9, 0, '30', NULL);

-- Chapter. 105
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(19, 1, ' الفيل', 'Al-Fil', 'The Elephant', 'Al-Fīl', 1, 5, 0, '30', NULL);

-- Chapter. 106
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(29, 1, 'قريش', 'Quraysh', 'Quraysh', 'Quraysh', 1, 4, 0, '30', NULL);

-- Chapter. 107
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(17, 1, 'الماعون', 'Al-Maun', 'The Small Kindnesses', 'Al-Ma`un', 1, 7, 0, '30', NULL);

-- Chapter. 108
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(15, 1, 'الكوثر', 'Al-Kawthar', 'The Abundance', 'Al-Kawthar', 1, 3, 0, '30', NULL);

-- Chapter. 109
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(18, 1, ' الكافرون', 'Al-Kafirun', 'The Disbelievers', 'Al-Kāfirūn', 1, 6, 0, '30', NULL);

-- Chapter. 110
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(114, 2, 'النصر', 'An-Nasr', 'Divine Support,', 'An-Nasr', 1, 3, 0, '30', NULL);

-- Chapter. 111
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(6, 1, 'المسد', 'Al-Masad', 'The Plaited Rope', 'Al-Masad', 1, 5, 0, '30', NULL);

-- Chapter. 112
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(22, 1, 'الإخلاص', 'Al-Ikhlas', 'Purity of Faith', '`Ikhlāş', 1, 4, 0, '30', NULL);

-- Chapter. 113
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(20, 1, ' الفلق', 'Al-Falaq', 'The Daybreak', 'Al-Falaq', 1, 5, 0, '30', NULL);

-- Chapter. 114
INSERT INTO QuranChapter (RevelationOrder, Classification, ArabicScriptName, ArabicName, EnglishName, Pronounciation, RukuhCount, VersesCount,  SajdahCount, Position, Muqattaat) VALUES(21, 1, 'الناس', 'An-Nas', 'Mankind', 'An-Nas', 1, 6, 0, '30', NULL);
