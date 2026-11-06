#include<bits/stdc++.h>
using namespace std;

string clean_ciphertext(string text){
    string result;
    for(char c:text){
        if(isalpha(c)) result+=toupper(c);
    }
    return result;
}

map<string,vector<int>> find_repeated_patterns(string text){
    map<string,vector<int>> patterns;

    for(int len=3;len<=5;len++){
        for(int i=0;i+len<=text.size();i++){
            string s=text.substr(i,len);
            patterns[s].push_back(i);
        }
    }

    return patterns;
}

vector<int> calculate_distances(map<string,vector<int>> patterns){
    vector<int> distances;

    for(auto p:patterns){
        vector<int> pos=p.second;

        if(pos.size()>1){
            for(int i=0;i<pos.size();i++){
                for(int j=i+1;j<pos.size();j++)
                    distances.push_back(pos[j]-pos[i]);
            }
        }
    }

    return distances;
}

map<int,int> find_factors(vector<int> distances){
    map<int,int> factors;

    for(int d:distances){
        for(int i=2;i<=20;i++){
            if(d%i==0)
                factors[i]++;
        }
    }

    return factors;
}

vector<int> kasiski_analysis(string text){
    auto patterns=find_repeated_patterns(text);
    auto distances=calculate_distances(patterns);
    auto factors=find_factors(distances);

    vector<pair<int,int>> temp;

    for(auto p:factors)
        temp.push_back({p.second,p.first});

    sort(temp.rbegin(),temp.rend());

    vector<int> candidates;

    for(auto p:temp)
        candidates.push_back(p.second);

    return candidates;
}

double calculate_ic(string text){
    int n=text.size();

    if(n<=1)
        return 0;

    int freq[26]={0};

    for(char c:text)
        freq[c-'A']++;

    double sum=0;

    for(int i=0;i<26;i++)
        sum+=freq[i]*(freq[i]-1);

    return sum/(n*(n-1));
}

vector<string> split_into_groups(string text,int keyLength){
    vector<string> groups(keyLength);

    for(int i=0;i<text.size();i++)
        groups[i%keyLength]+=text[i];

    return groups;
}

void frequency_analysis(string group){
    int freq[26]={0};

    for(char c:group)
        freq[c-'A']++;

    for(int i=0;i<26;i++)
        cout<<char('A'+i)<<":"<<freq[i]<<" ";

    cout<<endl;
}

int find_shift(string group){
    double english[26]={
        8.167,1.492,2.782,4.253,12.702,2.228,
        2.015,6.094,6.966,0.153,0.772,4.025,
        2.406,6.749,7.507,1.929,0.095,5.987,
        6.327,9.056,2.758,0.978,2.360,0.150,
        1.974,0.074
    };

    int freq[26]={0};

    for(char c:group)
        freq[c-'A']++;

    int n=group.size();
    double bestScore=1e18;
    int bestShift=0;

    for(int shift=0;shift<26;shift++){
        double score=0;

        for(int i=0;i<26;i++){
            int index=(i+shift)%26;
            double observed=freq[index];
            double expected=english[i]*n/100.0;

            if(expected>0)
                score+=(observed-expected)*(observed-expected)/expected;
        }

        if(score<bestScore){
            bestScore=score;
            bestShift=shift;
        }
    }

    return bestShift;
}

string find_key(string text,int keyLength){
    vector<string> groups=split_into_groups(text,keyLength);
    string key;

    for(string group:groups){
        int shift=find_shift(group);
        key+=char('A'+shift);
    }

    return key;
}

string vigenere_decrypt(string ciphertext,string key){
    string plaintext;

    for(int i=0;i<ciphertext.size();i++){
        int c=ciphertext[i]-'A';
        int k=key[i%key.size()]-'A';

        plaintext+=char('A'+(c-k+26)%26);
    }

    return plaintext;
}

string vigenere_encrypt(string plaintext,string key){
    string ciphertext;

    for(int i=0;i<plaintext.size();i++){
        int p=plaintext[i]-'A';
        int k=key[i%key.size()]-'A';

        ciphertext+=char('A'+(p+k)%26);
    }

    return ciphertext;
}

bool verify(string ciphertext,string plaintext,string key){
    return vigenere_encrypt(plaintext,key)==ciphertext;
}

int find_key_length(string text,vector<int> candidates){
    double bestIC=0;
    int bestLength=1;

    for(int keyLength:candidates){
        if(keyLength>20||keyLength>=text.size())
            continue;

        vector<string> groups=split_into_groups(text,keyLength);
        double total=0;

        for(string group:groups)
            total+=calculate_ic(group);

        double avg=total/keyLength;

        if(avg>bestIC){
            bestIC=avg;
            bestLength=keyLength;
        }
    }

    if(bestLength==1){
        for(int keyLength=2;keyLength<=20;keyLength++){
            vector<string> groups=split_into_groups(text,keyLength);
            double total=0;

            for(string group:groups)
                total+=calculate_ic(group);

            double avg=total/keyLength;

            if(avg>bestIC){
                bestIC=avg;
                bestLength=keyLength;
            }
        }
    }

    return bestLength;
}

int main(){
    ifstream file("ciphertext.txt");
    string ciphertext,line;

    if(!file){
        cout<<"File not found"<<endl;
        return 0;
    }

    while(getline(file,line))
        ciphertext+=line;
    file.close();

    ciphertext=clean_ciphertext(ciphertext);
    cout<<"Ciphertext length: "<<ciphertext.size()<<endl;

    vector<int> candidates=kasiski_analysis(ciphertext);
    cout<<"\nKasiski candidate key lengths:"<<endl;
    for(int x:candidates)
        cout<<x<<" ";
    cout<<endl;

    int keyLength=find_key_length(ciphertext,candidates);
    cout<<"\nEstimated key length: "<<keyLength<<endl;

    vector<string> groups=split_into_groups(ciphertext,keyLength);
    cout<<"\nFrequency tables:"<<endl;
    for(int i=0;i<groups.size();i++){
        cout<<"Group "<<i+1<<": ";
        frequency_analysis(groups[i]);
    }

    string key=find_key(ciphertext,keyLength);
    cout<<"\nRecovered key: "<<key<<endl;

    string plaintext=vigenere_decrypt(ciphertext,key);
    cout<<"\nRecovered plaintext:"<<endl;
    cout<<plaintext<<endl;

    cout<<"\nVerification: ";
    if(verify(ciphertext,plaintext,key))
        cout<<"SUCCESS"<<endl;
    else
        cout<<"FAILED"<<endl;

    return 0;
}
