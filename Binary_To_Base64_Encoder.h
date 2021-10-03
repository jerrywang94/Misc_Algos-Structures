#include <string>

// Binary to Base64 encoder
//
// Useful for converting long strings of binary
// into a shorter text format that users can copy around.
// Takes a string of binary as an input and converts
// every 6 bits into a Base64 char:
// 0-9: 0-9
// a-z: 10-35
// A-Z: 36-61
// = : 62
// + : 63
// Note that this pads the end with extra 0s if there isn't enough
// bits. Code can be easily adapted to pad the beginning instead.
std::string binary_to_64(std::string binary_input) {
  std::string result {};
  
	for (int i = 0; i < binary_input.size(); i = i + 6) {
		int agg_num = 0;
		for (int j = 0; j < 6; j++) {
			agg_num <<= 1;
			if (i + j < binary_input.size() && binary_input[i + j] == '1')
				agg_num += 1;
		}

		// 0-9: 0-9
		// a-z: 10-35
		// A-Z: 36-61
		// = : 62
		// + : 63
		if (agg_num < 10)
			result += std::to_string(agg_num);
		else if (agg_num < 36)
			result += ('a' + (agg_num - 10));
		else if (agg_num < 62)
			result += ('A' + (agg_num - 36));
		else if (agg_num == 63)
			result += '=';
		else
			result += '+';
	}

	return result;
}

// Base64 to Binary Decoder

std::string b64_to_binary(std::string b64_input){

  std::string result {};
  
	for (int i = 0; i < b64_input.size() ; i++) {
    
		int curr_num = 0;
		switch (b64_input[i]) {
		case '+':
			curr_num = 63;
			break;
		case '=':
			curr_num = 62;
			break;
		default:
			if (std::isdigit(b64_input[i]))
				curr_num = b64_input[i] - '0';
			else if (b64_input[i] >= 'a' && b64_input[i] <= 'z')
				curr_num = b64_input[i] - 'a' + 10;
			else if (b64_input[i] >= 'A' && b64_input[i] <= 'Z')
				curr_num = b64_input[i] - 'A' + 36;
      else
        return "invalid string";
			break;
		}

		int conv = 32;
		for (int j = 0; j < 6; j++) {
			if (curr_num >= conv) {
				result+='1';
				curr_num -= conv;
			}
      else
        result+='0';
			conv >>= 1;
		}
	}
  return result;
}
