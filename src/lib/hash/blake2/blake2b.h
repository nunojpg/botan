/*
* BLAKE2b
* (C) 2016 cynecx
*
* Botan is released under the Simplified BSD License (see license.txt)
*/

#ifndef BOTAN_BLAKE2B_H_
#define BOTAN_BLAKE2B_H_

#include <botan/hash.h>
#include <botan/sym_algo.h>
#include <string>
#include <memory>

namespace Botan {

class BLAKE2bMAC;

/**
* BLAKE2B
*/
class BLAKE2b final : public HashFunction, public SymmetricAlgorithm
   {
   public:
      /**
      * @param output_bits the output size of BLAKE2b in bits
      */
      explicit BLAKE2b(size_t output_bits = 512);

      size_t hash_block_size() const override { return 128; }
      size_t output_length() const override { return m_output_bits / 8; }
      size_t key_size() const { return m_key_size; }

      Key_Length_Specification key_spec() const override;

      std::unique_ptr<HashFunction> new_object() const override;
      std::string name() const override;
      void clear() override;

      std::unique_ptr<HashFunction> copy_state() const override;

   protected:
      friend class BLAKE2bMAC;

      void key_schedule(const uint8_t key[], size_t length) override;

      void add_data(const uint8_t input[], size_t length) override;
      void final_result(uint8_t out[]) override;

   private:
      void state_init();
      void compress(const uint8_t* data, size_t blocks, uint64_t increment);

      const size_t m_output_bits;

      secure_vector<uint8_t> m_buffer;
      size_t m_bufpos;

      secure_vector<uint64_t> m_H;
      uint64_t m_T[2];
      uint64_t m_F[2];

      size_t m_key_size;
      secure_vector<uint8_t> m_padded_key_buffer;
   };

}

#endif
