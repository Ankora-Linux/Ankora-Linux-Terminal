#include "lexer.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

static int ret = 0;
static std::string spesifik_ret = "?";
static bool error;
static std::string error_info;

static unsigned long int ifade_no = 1;

static void return_error(const std::string& error_information, const int returns, const std::string& spesifik_returns = "?BELİRTİLMEMİŞ?") {
    error = true;
    error_info = "İfade: " + std::to_string(ifade_no) + " - (" + error_information + ")";
    ret = returns;
    spesifik_ret = spesifik_returns;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Kullanım: " << argv[0] << " <girdi_dosyası> <çıktı_dosyası>" << std::endl;
        return -1;
    }

    std::ifstream input(argv[1], std::ios::binary);
    if (!input.is_open()) {
        std::cerr << "Girdi dosyası açılamadı!" << std::endl;
        return -1;
    }
    
    std::ofstream output(argv[2], std::ios::binary);
    if (!output.is_open()) {
        std::cerr << "Çıktı dosyası açılamadı!" << std::endl;
        return -1;
    }

    std::stringstream ss;
    ss << input.rdbuf();

    std::map<int, std::vector<std::string>> code1 = lexerfunc(ss);

    output << "LEXER: " << "\n\n";
    for (int line = 0; line < code1.size(); line++) {
        output << "Line: " << line+1 << "\n\n";
        std::vector<std::string> out = code1[line];

        
        if (out.size() < 2) {
            if (out[0] == "notepad") {
                std::fstream file(out[1]);

                if (!file.is_open()) {
                    return_error(
                        "Notepad: " + out[1] + " dosyası açılamadı!",
                        -1,
                        "NOTEPAD::BOZUK_DOSYA"
                    );
                }

                if (file.bad()) {
                    return_error(
                        "Notepad: " + out[1] + " dosyası bozuk görünüyor!",
                        -1,
                        "NOTEPAD::BOZUK_DOSYA"
                    );
                }

                if (error) break;

                // @miracsalih: "Buraya syscall veya metin tabanlı yazılım eklenebilir @Parsozkn."
            }
        }
    }
    output << "---";

    input.close();
    output.close();

    if (error) {
        std::cerr << error_info << "\n";
        std::cerr << "Terminal: " << ret << " döndü. " << spesifik_ret << "\n\n";
        return ret;
    } else {
        std::cout << "Terminal: Hata bulunamadı." << "\n";
        std::cout << "Terminal: 0 döndü." << "\n\n";
        return 0;
    }
}
