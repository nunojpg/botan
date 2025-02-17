/*
* DL Scheme
* (C) 1999-2007 Jack Lloyd
*
* Botan is released under the Simplified BSD License (see license.txt)
*/

#include <botan/dl_algo.h>
#include <botan/numthry.h>
#include <botan/der_enc.h>
#include <botan/ber_dec.h>

namespace Botan {

size_t DL_Scheme_PublicKey::key_length() const
   {
   return m_group.p_bits();
   }

size_t DL_Scheme_PublicKey::estimated_strength() const
   {
   return m_group.estimated_strength();
   }

AlgorithmIdentifier DL_Scheme_PublicKey::algorithm_identifier() const
   {
   return AlgorithmIdentifier(object_identifier(),
                              m_group.DER_encode(group_format()));
   }

std::vector<uint8_t> DL_Scheme_PublicKey::public_key_bits() const
   {
   std::vector<uint8_t> output;
   DER_Encoder(output).encode(m_y);
   return output;
   }

DL_Scheme_PublicKey::DL_Scheme_PublicKey(const DL_Group& group, const BigInt& y) :
   m_y(y),
   m_group(group)
   {
   }

DL_Scheme_PublicKey::DL_Scheme_PublicKey(const AlgorithmIdentifier& alg_id,
                                         const std::vector<uint8_t>& key_bits,
                                         DL_Group_Format format) :
   m_group(alg_id.parameters(), format)
   {
   BER_Decoder(key_bits).decode(m_y);
   }

secure_vector<uint8_t> DL_Scheme_PrivateKey::private_key_bits() const
   {
   return DER_Encoder().encode(m_x).get_contents();
   }

DL_Scheme_PrivateKey::DL_Scheme_PrivateKey(const AlgorithmIdentifier& alg_id,
                                           const secure_vector<uint8_t>& key_bits,
                                           DL_Group_Format format)
   {
   m_group.BER_decode(alg_id.parameters(), format);

   BER_Decoder(key_bits).decode(m_x);
   }

/*
* Check Public DL Parameters
*/
bool DL_Scheme_PublicKey::check_key(RandomNumberGenerator& rng,
                                    bool strong) const
   {
   if(!m_group.verify_public_element(m_y))
      return false;

   return m_group.verify_group(rng, strong);
   }

/*
* Check DL Scheme Private Parameters
*/
bool DL_Scheme_PrivateKey::check_key(RandomNumberGenerator& rng,
                                     bool strong) const
   {
   return m_group.verify_group(rng, strong) && m_group.verify_element_pair(m_y, m_x);
   }

const BigInt& DL_Scheme_PublicKey::get_int_field(const std::string& field) const
   {
   if(field == "p")
      return this->group_p();
   else if(field == "q")
      return this->group_q();
   else if(field == "g")
      return this->group_g();
   else if(field == "y")
      return this->get_y();
   else
      return Public_Key::get_int_field(field);
   }

const BigInt& DL_Scheme_PrivateKey::get_int_field(const std::string& field) const
   {
   if(field == "x")
      return this->get_x();
   else
      return DL_Scheme_PublicKey::get_int_field(field);
   }

}
