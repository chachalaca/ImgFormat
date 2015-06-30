/* 
 * File:   main.cpp
 * Author: f.blach@owi.cz
 * 
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "Huffman.h"

using namespace std;


/*
 * 
 */
int main(int argc, char **argv) {

    switch(argc) {
        case 1:
            cout << "encode [name of file to be encoded] [encoded file]" << endl;
            cout << "decode [name of file to be decoded] [name of decoded file]"<< endl;
            exit(0);
        case 2:
            cout << "missing source filename"<<endl;
            cout << "encode [name of file to be encoded] [encoded file]" << endl;
            cout << "decode [name of file to be decoded] [name of decoded file]"<< endl;
            exit(1);
        case 3:
            cout << "missing destination filename"<<endl;
            cout << "encode [name of file to be encoded] [encoded file]" << endl;
            cout << "decode [name of file to be decoded] [name of decoded file]"<< endl;
            exit(1);

    }




    if(strcmp(argv[1], "encode")==0) {

        std::ifstream is(argv[2]);



        std::vector<char> text;

        while (is.good()) {
            char c = is.get();
            if (is.good())
                text.push_back(c);
        }

        is.close();

        if(text[0] != 'P' || text[1] != '3') {
            cout << "PPM P3 only.";
            return 1;
        }



        Huffman<char> *h = new Huffman<char>();


        std::vector<bool> code = h->encode(&text);

        std::map<char, int> alphabet = h->extractAlphabet();

        unsigned short alphabetSize = (unsigned short)alphabet.size();

        short r = code.size() % 8;
        long bsize = ((code.size() - r) / 8);



        std::ofstream outfile(argv[3], std::ofstream::binary);


        outfile.write(reinterpret_cast<char *>(&alphabetSize), sizeof(alphabetSize));

        std::map<char, int>::iterator ai;

        for(ai = alphabet.begin(); ai != alphabet.end(); ai ++) {
            outfile.write(reinterpret_cast<const char *>(&(ai->first)), sizeof(ai->first));
            outfile.write(reinterpret_cast<const char *>(&(ai->second)), sizeof(ai->second));
        }


        unsigned char byte[1];
        unsigned char firstByte[1];

        firstByte[0] = 0; // 00000000

        if ((r - (r % 4)) / 4)
            firstByte[0] |= 1 << 2; // 00X00000

        if (((r % 4) - (r % 2)) / 2)
            firstByte[0] |= 1 << 1; // 0X000000

        if (r % 2)
            firstByte[0] |= 1; // X0000000

        outfile.write(reinterpret_cast<char *>(firstByte), 1);

        for (int y = 0; y < (bsize + 1); y++) { //byte
            byte[0] = 0;
            for (int i = 0; i < 8; ++i) { //bit
                if ((8 * y + i) <= code.size() && code[8 * y + i]) {
                    byte[0] |= 1 << i;
                }
            }
            outfile.write(reinterpret_cast<char *>(byte), 1);
        }


        outfile.close();
    }

    else if(strcmp(argv[1], "decode") == 0) {

        std::ifstream infile (argv[2],std::ifstream::binary);

        unsigned short as;

        char ch;

        int count;

        infile.read (reinterpret_cast<char *>(&as), sizeof(as));

        std::map<char, int> a;

        for(int i = 0; i < as; i++) {
            infile.read (reinterpret_cast<char *>(&ch), sizeof(ch));

            infile.read (reinterpret_cast<char *>(&count), sizeof(count));

            a[ch] = count;
        }

        char first;

        infile.read (&first, sizeof(first));

        int tail = sizeof(int) - (int)first;

        std::vector<bool> c;

        char buf;

        while (!infile.eof()) {
            infile.read (&buf, sizeof(buf));
            if(buf != 0) {
                for (int i = 0; i < 8; ++i) { //bit
                    if ((buf >> i) & 1) {
                        c.push_back(true);
                    }
                    else {
                        c.push_back(false);
                    }
                }
            }
        }

        for(int i=0; i<12+tail; i++) {
            c.erase(c.end());
        }


        Huffman<char> *hh = new Huffman<char>(a);

        std::vector<char> decodedText;
        decodedText = hh->decode(&c);

        std::ofstream outfile(argv[3], std::ofstream::out);

        std::vector<char>::iterator ci = decodedText.begin();
        while (ci != decodedText.end()) {
            cout << *ci;
            outfile << *ci;
            ci++;
        }
    }

    else {
        cout << "encode [name of file to be encoded] [encoded file]" << endl;
        cout << "decode [name of file to be decoded] [name of decoded file]"<< endl;
    }







    return 0;
}

