#ifndef CHAPTER_HH
#define CHAPTER_HH

#include <vector>
#include <map>
#include <string>
#include "core/logging.h"
#include "core/quran/verse.h"

namespace quran {
class Quran;
class Chapter : public el::Loggable {
public:
    enum class Classification : unsigned short { Meccan = 1, Medinan = 2 };
    enum class Name : unsigned short {
        AlFatiha = 1,
        AlBaqarah = 2,
        AlImran = 3,
        AnNisa = 4,
        AlMaida = 5,
        AlAnaam = 6,
        AlAaraaf = 7,
        AlAnfaal = 8,
        AtTawba = 9,
        Yunus = 10,
        Hud = 11,
        Yusuf = 12,
        ArRaad = 13,
        Ibrahim = 14,
        AlHijr = 15,
        AnNahl = 16,
        AlIsra = 17,
        AlKahaf = 18,
        Marayam = 19,
        Taha = 20,
        AlAnbiya = 21,
        AlHajj = 22,
        AlMuminun = 23,
        AnNur = 24,
        AlFurqan = 25,
        AshShuarah = 26,
        AnNaml = 27,
        AlQasas = 28,
        AlAnkabut = 29,
        ArRum = 30,
        Luqman = 31,
        AsSajda = 32,
        AlAhzaab = 33,
        Saba = 34,
        Faatir = 35,
        Yasin = 36,
        AsSaffaat = 37,
        Sad = 38,
        AzZummar = 39,
        Ghafir = 40,
        HaameemSajda = 41,
        AshShuhra = 42,
        AzZakhruf = 43,
        AdDukhan = 44,
        AlJathiya = 45,
        AlAhqaf = 46,
        Muhammad = 47,
        AlFath = 48,
        AlHujurat = 49,
        Qaf = 50,
        AdhDhariyat = 51,
        AtTur = 52,
        AnNajm = 53,
        AlQamar = 54,
        ArRahman = 55,
        AlWaqia = 56,
        AlHadid = 57,
        AlMujadila = 58,
        AlHashr = 59,
        AlMumtahna = 60,
        AsSaff = 61,
        AlJumuah = 62,
        AlMunafiqun = 63,
        AtTaghabun = 64,
        AtTalaq = 65,
        AtTahrim = 66,
        AlMulk = 67,
        AlQalam = 68,
        AlHaqqah = 69,
        AtMaiarij = 70,
        Nuh = 71,
        AlJinn = 72,
        AlMuzzammil = 73,
        AlMuddaththir = 74,
        AlQiyama = 75,
        AlInsan = 76,
        AlMursalat = 77,
        AnNabah = 78,
        AnNaziyat = 79,
        Aabasa = 80,
        AtTakwir = 81,
        AlIntifar = 82,
        AlMutaffifeen = 83,
        AlInshiqaq = 84,
        AlBuruj = 85,
        AlTariq = 86,
        AlAalaa = 87,
        AlGhashiya = 88,
        AlFajr = 89,
        AlBalad = 90,
        AsShams = 91,
        AlLail = 92,
        AdDhuha = 93,
        AshSharh = 94,
        AlTin = 95,
        AlAlaq = 96,
        AlQadar = 97,
        AlBaiyina = 98,
        AzZalzalah = 99,
        AlAdiyat = 100,
        AlQariyah = 101,
        AtTakathur = 102,
        AlAsr = 103,
        AlHumaza = 104,
        AlFil = 105,
        Quraysh = 106,
        AlMaun = 107,
        AlKauthar = 108,
        AlKafirun = 109,
        AnNasr = 110,
        AlMasad = 111,
        AlIkhlas = 112,
        AlFalaq = 113,
        AnNaas = 114
    };
    
    Chapter(Chapter::Classification classification, Chapter::Name name, std::wstring&& arabicScriptName, 
            std::wstring&& arabicName, std::wstring&& englishName, int rukuhCount, int versesCount, 
            int sajdahCount, std::vector<int>&& positions, std::wstring&& muqattaat);
    
    void log(el::base::type::ostream_t& os) const;
    
    Chapter::Classification classification(void) const;
    Chapter::Name name(void) const;
    const std::wstring& arabicScriptName(void) const;
    const std::wstring& arabicName(void) const;
    const std::wstring& englishName(void) const;
    const std::wstring& pronounciation(void) const;
    int rukuhCount(void) const;
    int versesCount(void) const;
    int sajdahCount(void) const;
    const std::vector<int>& positions(void) const;
    const std::wstring& muqattaat(void) const;
    const Verses& verses(void) const;
    const Verse* readVerse(int number) const;
private:
    friend class Quran;
    Classification m_classification;
    Name m_name;
    std::wstring m_arabicScriptName;
    std::wstring m_arabicName;
    std::wstring m_englishName;
    std::wstring m_pronounciation;
    int m_rukuhCount;
    int m_versesCount;
    int m_sajdahCount;
    std::vector<int> m_positions;
    std::wstring m_muqattaat;
    Verses m_verses;
};
typedef std::pair<int, Chapter> SingleChapter;
typedef std::map<int, Chapter> Chapters;
} // namespace quran
#endif // CHAPTER_HH
