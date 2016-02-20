/*
 * Copyright 1999-2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id: KVStringPair.hpp 191054 2005-06-17 02:56:35Z jberry $
 */

#if !defined(KVSTRINGPAIR_HPP)
#define KVSTRINGPAIR_HPP

#include <../../../SceneEdit/Xerces/src/xercesc/util/XMemory.hpp>
#include <../../../SceneEdit/Xerces/src/xercesc/util/PlatformUtils.hpp>

#include <xercesc/internal/XSerializable.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//
//  This class provides a commonly used data structure, which is that of
//  a pair of strings which represent a 'key=value' type mapping. It works
//  only in terms of XMLCh type raw strings.
//
class XMLUTIL_EXPORT KVStringPair : public XSerializable, public XMemory
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    KVStringPair(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    KVStringPair
    (
        const XMLCh* const key
        , const XMLCh* const value
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    KVStringPair
    (
        const XMLCh* const key
        , const XMLCh* const value
        , const unsigned int valueLength
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    KVStringPair
    (
        const XMLCh* const key
        , const unsigned int keyLength
        , const XMLCh* const value
        , const unsigned int valueLength
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    KVStringPair(const KVStringPair& toCopy);
    ~KVStringPair();


    // -----------------------------------------------------------------------
    //  Getters
    //
    //  We support the
    // -----------------------------------------------------------------------
    const XMLCh* getKey() const;
    XMLCh* getKey();
    const XMLCh* getValue() const;
    XMLCh* getValue();


    // -----------------------------------------------------------------------
    //  Setters
    // -----------------------------------------------------------------------
    void setKey(const XMLCh* const newKey);
    void setValue(const XMLCh* const newValue);
    void setKey
    (
        const   XMLCh* const newKey
        , const unsigned int newKeyLength
    );
    void setValue
    (
        const   XMLCh* const newValue
        , const unsigned int newValueLength
    );
    void set
    (
        const   XMLCh* const newKey
        , const XMLCh* const newValue
    );
    void set
    (
        const     XMLCh* const newKey
        , const   unsigned int newKeyLength
        , const   XMLCh* const newValue
        , const   unsigned int newValueLength
    );

    /***
     * Support for Serialization/De-serialization
     ***/
    DECL_XSERIALIZABLE(KVStringPair)

private :
    // unimplemented:
       
    KVStringPair& operator=(const KVStringPair&);
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fKey
    //      The string that represents the key field of this object.
    //
    //  fKeyAllocSize
    //      The amount of memory allocated for fKey.
    //
    //  fValue
    //      The string that represents the value of this pair object.
    //
    //  fValueAllocSize
    //      The amount of memory allocated for fValue.
    //
    // -----------------------------------------------------------------------
    unsigned long  fKeyAllocSize;
    unsigned long  fValueAllocSize;
    XMLCh*         fKey;
    XMLCh*         fValue;
    MemoryManager* fMemoryManager;
};

// ---------------------------------------------------------------------------
//  KVStringPair: Getters
// ---------------------------------------------------------------------------
inline const XMLCh* KVStringPair::getKey() const
{
    return fKey;
}

inline XMLCh* KVStringPair::getKey()
{
    return fKey;
}

inline const XMLCh* KVStringPair::getValue() const
{
    return fValue;
}

inline XMLCh* KVStringPair::getValue()
{
    return fValue;
}

// ---------------------------------------------------------------------------
//  KVStringPair: Setters
// ---------------------------------------------------------------------------
inline void KVStringPair::setKey(const XMLCh* const newKey)
{
    setKey(newKey, XMLString::stringLen(newKey));
}

inline void KVStringPair::setValue(const XMLCh* const newValue)
{
    setValue(newValue, XMLString::stringLen(newValue));
}

inline void KVStringPair::setKey(  const XMLCh* const newKey
                                 , const unsigned int newKeyLength)
{
    if (newKeyLength >= fKeyAllocSize)
    {
        fMemoryManager->deallocate(fKey); //delete [] fKey;
        fKeyAllocSize = newKeyLength + 1;
        fKey = (XMLCh*) fMemoryManager->allocate(fKeyAllocSize * sizeof(XMLCh)); //new XMLCh[fKeyAllocSize];
    }

    memcpy(fKey, newKey, (newKeyLength+1) * sizeof(XMLCh)); // len+1 because of the 0 at the end
}

inline void KVStringPair::setValue(  const XMLCh* const newValue
                                   , const unsigned int newValueLength)
{
    if (newValueLength >= fValueAllocSize)
    {
        fMemoryManager->deallocate(fValue); //delete [] fValue;
        fValueAllocSize = newValueLength + 1;
        fValue = (XMLCh*) fMemoryManager->allocate(fValueAllocSize * sizeof(XMLCh)); //new XMLCh[fValueAllocSize];
    }

    memcpy(fValue, newValue, (newValueLength+1) * sizeof(XMLCh)); // len+1 because of the 0 at the end
}

inline void KVStringPair::set(  const   XMLCh* const    newKey
                              , const   XMLCh* const    newValue)
{
    setKey(newKey, XMLString::stringLen(newKey));
    setValue(newValue, XMLString::stringLen(newValue));
}

inline void KVStringPair::set(  const   XMLCh* const newKey
                              , const   unsigned int newKeyLength
                              , const   XMLCh* const newValue
                              , const   unsigned int newValueLength)
{
    setKey(newKey, newKeyLength);
    setValue(newValue, newValueLength);
}


XERCES_CPP_NAMESPACE_END

#endif