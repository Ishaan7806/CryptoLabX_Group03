#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

// ============================================================
// GLOBAL DATA
// ============================================================

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Plaintext -> ciphertext encryption key
const string ORIGINAL_ENCRYPTION_KEY =
    "QWERTYUIOPASDFGHJKLZXCVBNM";

// Ciphertext -> plaintext inverse key
string ORIGINAL_DECRYPTION_KEY;

struct Iteration
{
    int step;
    string observation;
    char cipherLetter;
    char plainLetter;
    string tested;
    string result;
    string decision;
};

vector<Iteration> history;

// ============================================================
// BASIC FUNCTIONS
// ============================================================

char upperChar(char c)
{
    if (c >= 'a' && c <= 'z')
        return c - 'a';

    return c;
}

string toUpperString(string s)
{
    for (int i = 0; i < (int)s.length(); i++)
        s[i] = upperChar(s[i]);

    return s;
}

// ============================================================
// CREATE INVERSE KEY
// ============================================================

string inverseKey(string encryptionKey)
{
    string inverse = "??????????????????????????";

    for (int i = 0; i < 26; i++)
    {
        char plain = 'A' + i;
        char cipher = encryptionKey[i];

        inverse[cipher - 'A'] = plain;
    }

    return inverse;
}

// ============================================================
// ENCRYPTION
// plaintext -> ciphertext
// ============================================================

string encryptText(string text, string key)
{
    string result = text;

    for (int i = 0; i < (int)text.length(); i++)
    {
        char c = text[i];

        if (c >= 'a' && c <= 'z')
        {
            result[i] = key[c - 'a'];
        }
        else if (c >= 'A' && c <= 'Z')
        {
            result[i] = key[c - 'A'];
        }
        else
        {
            result[i] = c;
        }
    }

    return result;
}

// ============================================================
// APPLY DECRYPTION SUBSTITUTION
//
// key[ciphertext letter] = plaintext letter
// ============================================================

string apply_substitution(string ciphertext, string key)
{
    string result = ciphertext;

    for (int i = 0; i < (int)ciphertext.length(); i++)
    {
        char c = ciphertext[i];

        if (c >= 'A' && c <= 'Z')
        {
            if (key[c - 'A'] != '?')
                result[i] = key[c - 'A'];
            else
                result[i] = '_';
        }
        else if (c >= 'a' && c <= 'z')
        {
            char upper = c - 'a' + 'A';

            if (key[upper - 'A'] != '?')
                result[i] = key[upper - 'A'] - 'A' + 'a';
            else
                result[i] = '_';
        }
        else
        {
            result[i] = c;
        }
    }

    return result;
}

// ============================================================
// DISPLAY KEY
// ============================================================

void displayKey(string key)
{
    cout << "\nCipher : ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
    cout << "Plain  : " << key << "\n";
}

// ============================================================
// FREQUENCY ANALYSIS
// ============================================================

void frequency_analysis(string ciphertext)
{
    int freq[26] = {0};
    int total = 0;

    for (int i = 0; i < (int)ciphertext.length(); i++)
    {
        char c = upperChar(ciphertext[i]);

        if (c >= 'A' && c <= 'Z')
        {
            freq[c - 'A']++;
            total++;
        }
    }

    vector<pair<char, int> > data;

    for (int i = 0; i < 26; i++)
        data.push_back({'A' + i, freq[i]});

    sort(data.begin(), data.end(),
         [](pair<char, int> a, pair<char, int> b)
         {
             return a.second > b.second;
         });

    cout << "\n================================================\n";
    cout << "FREQUENCY ANALYSIS\n";
    cout << "================================================\n";

    cout << left
         << setw(10) << "Letter"
         << setw(10) << "Count"
         << setw(12) << "Percent"
         << "\n";

    for (int i = 0; i < 26; i++)
    {
        double percentage = 0;

        if (total > 0)
            percentage = (100.0 * data[i].second) / total;

        cout << left
             << setw(10) << data[i].first
             << setw(10) << data[i].second
             << fixed << setprecision(2)
             << setw(12) << percentage
             << "\n";
    }
}

// ============================================================
// EXTRACT WORDS
// ============================================================

vector<string> getWords(string text)
{
    vector<string> words;
    string current = "";

    for (int i = 0; i < (int)text.length(); i++)
    {
        char c = upperChar(text[i]);

        if (c >= 'A' && c <= 'Z')
        {
            current += c;
        }
        else
        {
            if (current.length() > 0)
            {
                words.push_back(current);
                current = "";
            }
        }
    }

    if (current.length() > 0)
        words.push_back(current);

    return words;
}

// ============================================================
// WORD FREQUENCY ANALYSIS
// ============================================================

void word_frequency_analysis(string ciphertext)
{
    vector<string> words = getWords(ciphertext);

    map<string, int> freq;

    for (int i = 0; i < (int)words.size(); i++)
        freq[words[i]]++;

    cout << "\n================================================\n";
    cout << "WORD FREQUENCY ANALYSIS\n";
    cout << "================================================\n";

    cout << "One-letter words:\n";

    for (map<string, int>::iterator it = freq.begin();
         it != freq.end(); ++it)
    {
        if (it->first.length() == 1)
        {
            cout << it->first
                 << " -> "
                 << it->second
                 << "\n";
        }
    }

    cout << "\nTwo-letter words:\n";

    for (map<string, int>::iterator it = freq.begin();
         it != freq.end(); ++it)
    {
        if (it->first.length() == 2)
        {
            cout << it->first
                 << " -> "
                 << it->second
                 << "\n";
        }
    }

    cout << "\nThree-letter words:\n";

    for (map<string, int>::iterator it = freq.begin();
         it != freq.end(); ++it)
    {
        if (it->first.length() == 3)
        {
            cout << it->first
                 << " -> "
                 << it->second
                 << "\n";
        }
    }

    cout << "\nRepeated words:\n";

    for (map<string, int>::iterator it = freq.begin();
         it != freq.end(); ++it)
    {
        if (it->second > 1)
        {
            cout << it->first
                 << " -> "
                 << it->second
                 << "\n";
        }
    }
}

// ============================================================
// PATTERN OF WORD
//
// Example:
//
// TEST -> 0123
// ALL  -> 011
// THAT -> 0120
// ============================================================

string wordPattern(string word)
{
    string pattern = "";
    map<char, int> seen;

    int next = 0;

    for (int i = 0; i < (int)word.length(); i++)
    {
        char c = word[i];

        if (seen.find(c) == seen.end())
        {
            seen[c] = next;
            next++;
        }

        pattern += char('0' + seen[c]);
    }

    return pattern;
}

// ============================================================
// PATTERN ANALYSIS
// ============================================================

void pattern_analysis(string ciphertext)
{
    vector<string> words = getWords(ciphertext);

    map<string, int> patterns;

    for (int i = 0; i < (int)words.size(); i++)
    {
        string p = wordPattern(words[i]);
        patterns[p]++;
    }

    cout << "\n================================================\n";
    cout << "PATTERN ANALYSIS\n";
    cout << "================================================\n";

    for (map<string, int>::iterator it = patterns.begin();
         it != patterns.end(); ++it)
    {
        if (it->second > 1)
        {
            cout << "Pattern "
                 << it->first
                 << " occurs "
                 << it->second
                 << " times\n";
        }
    }
}

// ============================================================
// COMMON ENGLISH WORDS
//
// Used only as language evidence for candidate substitutions.
// ============================================================

vector<string> commonWords()
{
    vector<string> words;

    string data[] =
    {
        "THE","OF","AND","TO","IN","A","IS","THAT","FOR","IT",
        "AS","WAS","WITH","BE","BY","ON","NOT","HE","I","THIS",
        "ARE","OR","HIS","FROM","AT","WHICH","BUT","HAVE","AN",
        "HAD","THEY","YOU","ONE","WORD","WE","CAN","ALL","HER",
        "HAS","THERE","WOULD","THEIR","WHAT","SO","UP","OUT",
        "IF","ABOUT","WHO","GET","WHEN","MAKE","TIME","NO","JUST",
        "KNOW","TAKE","INTO","YEAR","YOUR","GOOD","SOME","COULD",
        "THEM","SEE","OTHER","THAN","THEN","NOW","LOOK","ONLY",
        "COME","ITS","OVER","THINK","ALSO","BACK","AFTER","USE",
        "TWO","HOW","OUR","WORK","FIRST","WELL","WAY","EVEN",
        "NEW","WANT","BECAUSE","THESE","GIVE","DAY","MOST","US",
        "SYSTEM","SECURITY","CRYPTOGRAPHY","ENCRYPTION","DECRYPTION",
        "KEY","KEYS","CIPHER","CIPHERTEXT","PLAINTEXT","MESSAGE",
        "ALGORITHM","ATTACK","ATTACKER","SECRET","PUBLIC","PRIVATE",
        "COMPUTER","COMMUNICATION","DATA","INFORMATION","DIGITAL",
        "SIGNATURE","PROTOCOL","FUNCTION","MATHEMATICAL","SECURE",
        "SECURITY","AUTHENTICATION","RANDOM","RANDOMNESS","PROBABILITY",
        "NUMBER","NUMBERS","BIT","BITS","BLOCK","BLOCKS","STRING",
        "LETTER","LETTERS","SUBSTITUTION","MONOALPHABETIC","FREQUENCY",
        "ANALYSIS","METHOD","METHODS","PROBLEM","PROBLEMS","RESULT",
        "RESULTS","MESSAGE","MESSAGES","ATTACKS","KNOWN","UNKNOWN",
        "EXAMPLE","EXAMPLES","SIMPLE","COMPLEX","MODEL","MODELS",
        "DESIGN","DESIGNED","CONSTRUCT","CONSTRUCTION","PROTOCOLS",
        "CRYPTOGRAPHIC","CRYPTOGRAPHY","ENCRYPT","DECRYPT","ENCODE",
        "DECODE","AUTHENTICATE","AUTHENTICATION","VERIFICATION",
        "VERIFY","SECURE","SECURITY","PROTECT","PROTECTION",
        "ADVERSARY","ADVERSARIES","THREAT","THREATS","COMPUTATIONAL",
        "COMPUTATION","THEORETICAL","THEORY","PROOF","PROPERTIES",
        "PROPERTY","CONDITION","CONDITIONS","ATTEMPT","ATTEMPTS",
        "POSSIBLE","IMPOSSIBLE","EFFICIENT","EFFICIENCY","RISK",
        "RANDOMIZED","DISTRIBUTION","DISTRIBUTIONS","PROBABILITY",
        "PROBABILISTIC","FUNCTIONS","INPUT","OUTPUT","VALUE","VALUES",
        "SET","SETS","ELEMENT","ELEMENTS","GROUP","GROUPS","FIELD",
        "FIELDS","INTEGER","INTEGERS","PRIME","PRIMES","MODULO",
        "MODULAR","OPERATION","OPERATIONS","CALCULATE","CALCULATION",
        "CONSIDER","CONSIDERED","IMPORTANT","IMPORTANTLY","ASSUME",
        "ASSUMPTION","ASSUMPTIONS","SECURELY","GENERATE","GENERATION",
        "GENERATED","COMPARISON","COMPARE","DIFFERENT","SAME",
        "CORRECT","INCORRECT","RECOVER","RECOVERED","RECOVERY"
    };

    int n = sizeof(data) / sizeof(data[0]);

    for (int i = 0; i < n; i++)
        words.push_back(data[i]);

    return words;
}

// ============================================================
// CHECK WHETHER A WORD MATCHES CURRENT KEY
// ============================================================

bool wordCompatible(
    string cipherWord,
    string plainWord,
    string key)
{
    if (cipherWord.length() != plainWord.length())
        return false;

    char reverseMap[26];

    for (int i = 0; i < 26; i++)
        reverseMap[i] = '?';

    for (int i = 0; i < (int)cipherWord.length(); i++)
    {
        char c = cipherWord[i];
        char p = plainWord[i];

        if (key[c - 'A'] != '?' &&
            key[c - 'A'] != p)
        {
            return false;
        }

        if (reverseMap[p - 'A'] != '?' &&
            reverseMap[p - 'A'] != c)
        {
            return false;
        }

        reverseMap[p - 'A'] = c;
    }

    return true;
}

// ============================================================
// APPLY TEMPORARY MAPPING
// ============================================================

bool addMapping(
    string &key,
    char cipher,
    char plain)
{
    if (key[cipher - 'A'] != '?' &&
        key[cipher - 'A'] != plain)
        return false;

    for (int i = 0; i < 26; i++)
    {
        if (i != cipher - 'A' &&
            key[i] == plain)
            return false;
    }

    key[cipher - 'A'] = plain;

    return true;
}

// ============================================================
// SCORE PARTIAL PLAINTEXT
// ============================================================

double scoreText(string text)
{
    double score = 0;

    string common[] =
    {
        "THE","AND","ING","ION","TH","HE","IN","ER","AN","RE",
        "ON","AT","EN","ND","TI","ES","OR","TE","OF","ED",
        "IS","IT","AL","AR","ST","TO","NT","NG","SE","HA",
        "AS","OU","IO","LE","VE","CO","ME","DE","HI","RI",
        "RO","IC","NE","EA","RA","CE","LI","CH","LL","BE"
    };

    int n = sizeof(common) / sizeof(common[0]);

    for (int i = 0; i < n; i++)
    {
        string pattern = common[i];

        for (int j = 0;
             j + (int)pattern.length() <= (int)text.length();
             j++)
        {
            bool match = true;

            for (int k = 0; k < (int)pattern.length(); k++)
            {
                if (upperChar(text[j + k]) != pattern[k])
                {
                    match = false;
                    break;
                }
            }

            if (match)
                score += pattern.length();
        }
    }

    return score;
}

// ============================================================
// DISPLAY PARTIAL PLAINTEXT
// ============================================================

void display_partial_plaintext(
    string ciphertext,
    string key)
{
    string partial =
        apply_substitution(ciphertext, key);

    cout << "\n------------------------------------------------\n";
    cout << "PARTIAL PLAINTEXT\n";
    cout << "------------------------------------------------\n";

    int limit = 1000;

    if ((int)partial.length() < limit)
        limit = partial.length();

    for (int i = 0; i < limit; i++)
        cout << partial[i];

    if ((int)partial.length() > limit)
        cout << "\n...[truncated]...";

    cout << "\n------------------------------------------------\n";
}

// ============================================================
// FIND MOST FREQUENT UNUSED CIPHER LETTER
// ============================================================

char mostFrequentUnused(
    string ciphertext,
    string key)
{
    int freq[26] = {0};

    for (int i = 0; i < (int)ciphertext.length(); i++)
    {
        char c = upperChar(ciphertext[i]);

        if (c >= 'A' && c <= 'Z')
            freq[c - 'A']++;
    }

    int best = -1;
    int bestFreq = -1;

    for (int i = 0; i < 26; i++)
    {
        if (key[i] == '?' &&
            freq[i] > bestFreq)
        {
            best = i;
            bestFreq = freq[i];
        }
    }

    if (best == -1)
        return '?';

    return 'A' + best;
}

// ============================================================
// FIND MOST FREQUENT UNUSED PLAIN LETTER
// ============================================================

char mostFrequentUnusedPlain(string key)
{
    string english =
        "ETAOINSHRDLCUMWFGYPBVKJXQZ";

    for (int i = 0; i < (int)english.length(); i++)
    {
        char p = english[i];

        bool used = false;

        for (int j = 0; j < 26; j++)
        {
            if (key[j] == p)
            {
                used = true;
                break;
            }
        }

        if (!used)
            return p;
    }

    return '?';
}

// ============================================================
// RECORD ITERATION
// ============================================================

void recordIteration(
    string observation,
    char cipher,
    char plain,
    string tested,
    string result,
    string decision)
{
    Iteration item;

    item.step = history.size() + 1;
    item.observation = observation;
    item.cipherLetter = cipher;
    item.plainLetter = plain;
    item.tested = tested;
    item.result = result;
    item.decision = decision;

    history.push_back(item);
}

// ============================================================
// AUTOMATED FREQUENCY PHASE
// ============================================================

void frequencyPhase(
    string ciphertext,
    string &key)
{
    string english =
        "ETAOINSHRDLCUMWFGYPBVKJXQZ";

    int freq[26] = {0};

    for (int i = 0; i < (int)ciphertext.length(); i++)
    {
        char c = upperChar(ciphertext[i]);

        if (c >= 'A' && c <= 'Z')
            freq[c - 'A']++;
    }

    vector<int> order;

    for (int i = 0; i < 26; i++)
        order.push_back(i);

    sort(order.begin(), order.end(),
         [&](int a, int b)
         {
             return freq[a] > freq[b];
         });

    int count = 0;

    for (int i = 0;
         i < 26 && count < 12;
         i++)
    {
        int cipherIndex = order[i];

        if (freq[cipherIndex] == 0)
            continue;

        char cipher = 'A' + cipherIndex;

        char plain = english[count];

        if (addMapping(key, cipher, plain))
        {
            string partial =
                apply_substitution(ciphertext, key);

            recordIteration(
                "High-frequency ciphertext letter",
                cipher,
                plain,
                string(1, cipher) + " -> " + string(1, plain),
                partial.substr(0,
                    partial.length() > 120 ?
                    120 : partial.length()),
                "Accepted as frequency candidate"
            );

            cout << "\nStep "
                 << history.back().step
                 << ": "
                 << cipher
                 << " -> "
                 << plain
                 << "\n";

            display_partial_plaintext(ciphertext, key);

            count++;
        }
    }
}

// ============================================================
// WORD PATTERN BASED PHASE
// ============================================================

void patternPhase(
    string ciphertext,
    string &key)
{
    vector<string> cipherWords =
        getWords(ciphertext);

    vector<string> dictionary =
        commonWords();

    for (int w = 0;
         w < (int)cipherWords.size();
         w++)
    {
        string cw = cipherWords[w];

        if (cw.length() < 2 ||
            cw.length() > 12)
            continue;

        for (int d = 0;
             d < (int)dictionary.size();
             d++)
        {
            string pw = dictionary[d];

            if (pw.length() != cw.length())
                continue;

            if (wordPattern(cw) != wordPattern(pw))
                continue;

            if (!wordCompatible(cw, pw, key))
                continue;

            string oldKey = key;
            bool valid = true;

            for (int i = 0;
                 i < (int)cw.length();
                 i++)
            {
                if (!addMapping(
                        key,
                        cw[i],
                        pw[i]))
                {
                    valid = false;
                    break;
                }
            }

            if (!valid)
            {
                key = oldKey;
                continue;
            }

            string partial =
                apply_substitution(ciphertext, key);

            recordIteration(
                "Word pattern matched: " +
                cw +
                " has pattern " +
                wordPattern(cw),
                cw[0],
                pw[0],
                cw + " -> " + pw,
                partial.substr(0,
                    partial.length() > 120 ?
                    120 : partial.length()),
                "Accepted"
            );

            cout << "\nStep "
                 << history.back().step
                 << ": Pattern "
                 << cw
                 << " -> "
                 << pw
                 << "\n";

            display_partial_plaintext(ciphertext, key);

            break;
        }
    }
}

// ============================================================
// SHORT WORD PHASE
// ============================================================

void shortWordPhase(
    string ciphertext,
    string &key)
{
    vector<string> words =
        getWords(ciphertext);

    for (int i = 0; i < (int)words.size(); i++)
    {
        string word = words[i];

        if (word.length() == 1)
        {
            char cipher = word[0];

            if (key[cipher - 'A'] != '?')
                continue;

            // In English one-letter words are normally A or I.
            char candidates[2] = {'A', 'I'};

            for (int j = 0; j < 2; j++)
            {
                string oldKey = key;

                if (addMapping(
                        key,
                        cipher,
                        candidates[j]))
                {
                    string partial =
                        apply_substitution(
                            ciphertext,
                            key);

                    double score =
                        scoreText(partial);

                    recordIteration(
                        "One-letter word detected",
                        cipher,
                        candidates[j],
                        string(1, cipher) +
                        " -> " +
                        candidates[j],
                        partial.substr(0,
                            partial.length() > 120 ?
                            120 : partial.length()),
                        "Accepted"
                    );

                    cout << "\nStep "
                         << history.back().step
                         << ": "
                         << cipher
                         << " -> "
                         << candidates[j]
                         << " score = "
                         << score
                         << "\n";

                    display_partial_plaintext(
                        ciphertext,
                        key);

                    break;
                }

                key = oldKey;
            }
        }
    }
}

// ============================================================
// COMPLETE REMAINING KEY USING FREQUENCY
// ============================================================

void completeFrequencyKey(
    string ciphertext,
    string &key)
{
    while (true)
    {
        char cipher =
            mostFrequentUnused(
                ciphertext,
                key);

        if (cipher == '?')
            break;

        char plain =
            mostFrequentUnusedPlain(key);

        if (plain == '?')
            break;

        if (addMapping(
                key,
                cipher,
                plain))
        {
            string partial =
                apply_substitution(
                    ciphertext,
                    key);

            recordIteration(
                "Remaining unmapped letters completed by frequency order",
                cipher,
                plain,
                string(1, cipher) +
                " -> " +
                string(1, plain),
                partial.substr(0,
                    partial.length() > 120 ?
                    120 : partial.length()),
                "Accepted as completion"
            );
        }
    }
}

// ============================================================
// REPAIR USING ORIGINAL PLAINTEXT
//
// IMPORTANT:
// This function is ONLY validation.
// It does NOT participate in the attack.
// ============================================================

bool compareWithOriginal(
    string recovered,
    string original)
{
    int total = max(
        recovered.length(),
        original.length());

    int different = 0;

    for (int i = 0; i < total; i++)
    {
        char a = '?';
        char b = '?';

        if (i < (int)recovered.length())
            a = recovered[i];

        if (i < (int)original.length())
            b = original[i];

        if (a != b)
            different++;
    }

    cout << "\n================================================\n";
    cout << "FINAL PLAINTEXT VALIDATION\n";
    cout << "================================================\n";

    cout << "Original length  : "
         << original.length()
         << "\n";

    cout << "Recovered length : "
         << recovered.length()
         << "\n";

    cout << "Different chars  : "
         << different
         << "\n";

    if (recovered == original)
    {
        cout << "\nRESULT: EXACT MATCH\n";
        return true;
    }

    cout << "\nRESULT: NOT AN EXACT MATCH\n";

    cout << "\nFirst differences:\n";

    int shown = 0;

    for (int i = 0;
         i < total && shown < 20;
         i++)
    {
        char a = '?';
        char b = '?';

        if (i < (int)recovered.length())
            a = recovered[i];

        if (i < (int)original.length())
            b = original[i];

        if (a != b)
        {
            cout << "Position "
                 << i
                 << ": recovered='"
                 << a
                 << "' original='"
                 << b
                 << "'\n";

            shown++;
        }
    }

    return false;
}

// ============================================================
// VERIFY BY RE-ENCRYPTION
// ============================================================

bool verifyReEncryption(
    string recoveredPlaintext,
    string ciphertext,
    string recoveredKey)
{
    string encryptionKey =
        "??????????????????????????";

    for (int cipherIndex = 0;
         cipherIndex < 26;
         cipherIndex++)
    {
        char plain =
            recoveredKey[cipherIndex];

        if (plain == '?')
            return false;

        int plainIndex =
            plain - 'A';

        if (encryptionKey[plainIndex] != '?')
            return false;

        encryptionKey[plainIndex] =
            'A' + cipherIndex;
    }

    string regenerated =
        encryptText(
            recoveredPlaintext,
            encryptionKey);

    return regenerated == ciphertext;
}

// ============================================================
// SAVE TABLE
// ============================================================

void saveTable()
{
    ofstream csv(
        "cryptanalysis_table.csv");

    csv << "Step,Observation,Possible Substitution,"
           "Substitution Tested,Result,Decision\n";

    for (int i = 0;
         i < (int)history.size();
         i++)
    {
        Iteration &x = history[i];

        csv << x.step << ","
            << "\""
            << x.observation
            << "\","
            << "\""
            << x.cipherLetter
            << " -> "
            << x.plainLetter
            << "\","
            << "\""
            << x.tested
            << "\","
            << "\""
            << x.result
            << "\","
            << "\""
            << x.decision
            << "\"\n";
    }

    csv.close();

    ofstream txt(
        "cryptanalysis_table.txt");

    txt << "CRYPTANALYSIS ITERATION TABLE\n";
    txt << "============================================================\n\n";

    for (int i = 0;
         i < (int)history.size();
         i++)
    {
        Iteration &x = history[i];

        txt << "Step " << x.step << "\n";
        txt << "Observation: "
            << x.observation << "\n";

        txt << "Possible Substitution: "
            << x.cipherLetter
            << " -> "
            << x.plainLetter
            << "\n";

        txt << "Substitution Tested: "
            << x.tested << "\n";

        txt << "Result: "
            << x.result << "\n";

        txt << "Decision: "
            << x.decision << "\n";

        txt << "------------------------------------------------------------\n";
    }

    txt.close();

    cout << "\nCryptanalysis tables saved:\n";
    cout << "  cryptanalysis_table.csv\n";
    cout << "  cryptanalysis_table.txt\n";
}

// ============================================================
// SAVE RECOVERED PLAINTEXT
// ============================================================

void saveRecoveredPlaintext(
    string recovered)
{
    ofstream out(
        "recovered_plaintext.txt");

    out << recovered;

    out.close();
}

// ============================================================
// MAIN
// ============================================================

int main()
{
    srand(time(0));

    cout << "====================================================\n";
    cout << " MONOALPHABETIC SUBSTITUTION CIPHER\n";
    cout << " AUTOMATED FREQUENCY + PATTERN CRYPTANALYSIS\n";
    cout << "====================================================\n";

    // --------------------------------------------------------
    // READ ORIGINAL PLAINTEXT
    // --------------------------------------------------------

    ifstream input("plaintext.txt");

    if (!input)
    {
        cout << "\nERROR: plaintext.txt not found.\n";
        cout << "Place plaintext.txt in the same directory.\n";
        return 1;
    }

    string plaintext;
    string line;

    while (getline(input, line))
    {
        plaintext += line;
        plaintext += "\n";
    }

    input.close();

    cout << "\nPlaintext loaded.\n";
    cout << "Plaintext characters: "
         << plaintext.length()
         << "\n";

    // --------------------------------------------------------
    // ENCRYPTION
    // --------------------------------------------------------

    cout << "\nOriginal encryption key:\n";

    cout << "Plain : ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
    cout << "Cipher: "
         << ORIGINAL_ENCRYPTION_KEY
         << "\n";

    ORIGINAL_DECRYPTION_KEY =
        inverseKey(
            ORIGINAL_ENCRYPTION_KEY);

    cout << "\nActual inverse key used for validation:\n";

    cout << "Cipher: ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
    cout << "Plain : "
         << ORIGINAL_DECRYPTION_KEY
         << "\n";

    string ciphertext =
        encryptText(
            plaintext,
            ORIGINAL_ENCRYPTION_KEY);

    ofstream cipherFile(
        "ciphertext.txt");

    cipherFile << ciphertext;

    cipherFile.close();

    cout << "\nCiphertext generated.\n";
    cout << "Saved as ciphertext.txt\n";

    // --------------------------------------------------------
    // REQUIRED ANALYSIS
    // --------------------------------------------------------

    frequency_analysis(ciphertext);

    word_frequency_analysis(ciphertext);

    pattern_analysis(ciphertext);

    // --------------------------------------------------------
    // INITIAL UNKNOWN KEY
    // --------------------------------------------------------

    string recoveredKey =
        "??????????????????????????";

    cout << "\n====================================================\n";
    cout << "STARTING AUTOMATED CRYPTANALYSIS\n";
    cout << "====================================================\n";

    displayKey(recoveredKey);

    // --------------------------------------------------------
    // ITERATION 1: FREQUENCY
    // --------------------------------------------------------

    cout << "\n\n******** FREQUENCY PHASE ********\n";

    frequencyPhase(
        ciphertext,
        recoveredKey);

    // --------------------------------------------------------
    // ITERATION 2: SHORT WORDS
    // --------------------------------------------------------

    cout << "\n\n******** SHORT WORD PHASE ********\n";

    shortWordPhase(
        ciphertext,
        recoveredKey);

    // --------------------------------------------------------
    // ITERATION 3: PATTERN ANALYSIS
    // --------------------------------------------------------

    cout << "\n\n******** PATTERN PHASE ********\n";

    patternPhase(
        ciphertext,
        recoveredKey);

    // --------------------------------------------------------
    // COMPLETE REMAINING LETTERS
    // --------------------------------------------------------

    cout << "\n\n******** KEY COMPLETION PHASE ********\n";

    completeFrequencyKey(
        ciphertext,
        recoveredKey);

    // --------------------------------------------------------
    // FINAL KEY
    // --------------------------------------------------------

    cout << "\n====================================================\n";
    cout << "RECOVERED SUBSTITUTION KEY\n";
    cout << "====================================================\n";

    displayKey(recoveredKey);

    // --------------------------------------------------------
    // RECOVERED PLAINTEXT
    // --------------------------------------------------------

    string recoveredPlaintext =
        apply_substitution(
            ciphertext,
            recoveredKey);

    cout << "\n====================================================\n";
    cout << "RECOVERED PLAINTEXT\n";
    cout << "====================================================\n";

    cout << recoveredPlaintext << "\n";

    // --------------------------------------------------------
    // RE-ENCRYPTION VALIDATION
    // --------------------------------------------------------

    bool cipherMatch =
        verifyReEncryption(
            recoveredPlaintext,
            ciphertext,
            recoveredKey);

    cout << "\n====================================================\n";
    cout << "CIPHERTEXT RE-ENCRYPTION CHECK\n";
    cout << "====================================================\n";

    if (cipherMatch)
        cout << "PASS: Re-encrypted plaintext matches ciphertext.\n";
    else
        cout << "FAIL: Re-encryption does not match ciphertext.\n";

    // --------------------------------------------------------
    // FINAL ORIGINAL PLAINTEXT COMPARISON
    // --------------------------------------------------------

    bool plaintextMatch =
        compareWithOriginal(
            recoveredPlaintext,
            plaintext);

    // --------------------------------------------------------
    // SAVE OUTPUT
    // --------------------------------------------------------

    saveRecoveredPlaintext(
        recoveredPlaintext);

    saveTable();

    // --------------------------------------------------------
    // FINAL REPORT
    // --------------------------------------------------------

    cout << "\n====================================================\n";
    cout << "FINAL RESULT\n";
    cout << "====================================================\n";

    cout << "Recovered key : "
         << recoveredKey
         << "\n";

    cout << "Expected key  : "
         << ORIGINAL_DECRYPTION_KEY
         << "\n";

    if (recoveredKey ==
        ORIGINAL_DECRYPTION_KEY)
    {
        cout << "\nKEY RESULT: EXACT MATCH\n";
    }
    else
    {
        cout << "\nKEY RESULT: DIFFERENT FROM EXPECTED KEY\n";
    }

    if (plaintextMatch)
        cout << "PLAINTEXT RESULT: EXACT MATCH\n";
    else
        cout << "PLAINTEXT RESULT: NOT EXACT\n";

    cout << "\nFiles generated:\n";
    cout << "1. ciphertext.txt\n";
    cout << "2. recovered_plaintext.txt\n";
    cout << "3. cryptanalysis_table.csv\n";
    cout << "4. cryptanalysis_table.txt\n";

    cout << "\n====================================================\n";
    cout << "PROGRAM COMPLETE\n";
    cout << "====================================================\n";

    return 0;
}