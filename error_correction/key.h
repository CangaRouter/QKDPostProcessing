#ifndef KEY_H
#define KEY_H

#include <memory>
#include <string>
#include <vector>

namespace Cascade {

    class Key
    {
    public:
        Key(int nr_bits);
        Key(const Key& key);

        Key(int nrBits, int nrWords,std::vector< uint64_t>words);

        ~Key();
        static Key parseKey(std::string key);
        std::string to_string() const;
        int get_nr_bits() const;
        bool get_bit(int bit_nr) const;
        void set_bit(int bit_nr, int value);
        void flip_bit(int bit_nr);
        void apply_noise(double bit_error_rate);
        int compute_range_parity(int start_bit_nr, int end_bit_nr) const;
        int nr_bits_different(const Key& other_key) const;
        static std::string arrayToString(uint64_t* wordss, int size);
    private:
        int nr_bits;
        int nr_words;
        std::vector<uint64_t> words;
    public:
        int getNrWords() const;

       std::vector< uint64_t >getWords() const;

        void setNrWords(int nrWords);

        void setWords(std::vector<uint64_t >words);
    };

    typedef std::shared_ptr<Key> KeyPtr;

} /* namespace Cascade */

#endif /* ifndef KEY_H */
