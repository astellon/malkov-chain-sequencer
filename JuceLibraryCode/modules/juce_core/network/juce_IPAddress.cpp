/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

IPAddress::IPAddress() noexcept
{
    for (int i = 0; i < 16; ++i)
        address[i] = 0;
}

IPAddress::IPAddress (const uint8 bytes[], bool IPv6) noexcept : isIPv6 (IPv6)
{
    for (int i = 0; i < (isIPv6 ? 16 : 4); ++i)
        address[i] = bytes[i];

    if (! isIPv6)
        zeroUnusedBytes();
}

IPAddress::IPAddress (const uint16 bytes[8]) noexcept : isIPv6 (true)
{
    ByteUnion temp;

    for (int i = 0; i < 8; ++i)
    {
        temp.combined = bytes[i];

        address[i * 2]     = temp.split[0];
        address[i * 2 + 1] = temp.split[1];
    }
}

IPAddress::IPAddress (uint8 a0, uint8 a1, uint8 a2, uint8 a3) noexcept : isIPv6 (false)
{
    address[0] = a0;  address[1] = a1;
    address[2] = a2;  address[3] = a3;

    zeroUnusedBytes();
}

IPAddress::IPAddress (uint16 a1, uint16 a2, uint16 a3, uint16 a4,
                      uint16 a5, uint16 a6, uint16 a7, uint16 a8) noexcept : isIPv6 (true)

{
    uint16 array[8] = { a1, a2, a3, a4, a5, a6, a7, a8 };

    ByteUnion temp;

    for (int i = 0; i < 8; ++i)
    {
        temp.combined = array[i];
        address[i * 2]     = temp.split[0];
        address[i * 2 + 1] = temp.split[1];
    }
}

IPAddress::IPAddress (uint32 n) noexcept : isIPv6 (false)
{
    address[0] = (n >> 24);
    address[1] = (n >> 16) & 255;
    address[2] = (n >> 8) & 255;
    address[3] = (n & 255);

    zeroUnusedBytes();
}

static String removePort (const String& adr)
{
    if (adr.containsAnyOf ("[]"))
        return adr.fromFirstOccurrenceOf ("[", false, true).upToLastOccurrenceOf ("]", false, true);
    else if (adr.indexOf (":") == adr.lastIndexOf (":"))
        return adr.upToLastOccurrenceOf (":", false, true);

    return adr;
}

IPAddress::IPAddress (const String& adr)
{
    auto ipAddress = removePort (adr);

    isIPv6 = ipAddress.contains (":");

    if (! isIPv6)
    {
        auto tokens = StringArray::fromTokens (ipAddress, ".", {});

        for (int i = 0; i < 4; ++i)
            address[i] = (uint8) tokens[i].getIntValue();

        zeroUnusedBytes();
    }
    else
    {
        auto tokens = StringArray::fromTokens (ipAddress, ":", {});

        if (tokens.contains ({})) // if :: shorthand has been used
        {
            auto idx = tokens.indexOf ({});
            tokens.set (idx, "0");
            tokens.removeEmptyStrings();

            // mapped IPv4 address will be treated as a single token, so pad the end of the StringArray
            if (tokens[tokens.size() - 1].containsChar ('.'))
                tokens.add ({});

            while (tokens.size() < 8)
                tokens.insert (idx, "0");
        }

        for (int i = 0; i < 8; ++i)
        {
            if (i == 6 && isIPv4MappedAddress (IPAddress (address, true)))
            {
                IPAddress v4Address (tokens[i]);

                address[12] = v4Address.address[0];
                address[13] = v4Address.address[1];
                address[14] = v4Address.address[2];
                address[15] = v4Address.address[3];

                break;
            }

            ByteUnion temp;
            temp.combined = (uint16) CharacterFunctions::HexParser<int>::parse (tokens[i].getCharPointer());

            address[i * 2]     = temp.split[0];
            address[i * 2 + 1] = temp.split[1];
        }
    }
}

String IPAddress::toString() const
{
    if (! isIPv6)
    {
        String s ((int) address[0]);

        for (int i = 1; i < 4; ++i)
            s << '.' << (int) address[i];

        return s;
    }

    ByteUnion temp;

    temp.split[0] = address[0];
    temp.split[1] = address[1];

    auto addressString = String::toHexString (temp.combined);

    for (int i = 1; i < 8; ++i)
    {
        temp.split[0] = address[i * 2];
        temp.split[1] = address[i * 2 + 1];

        addressString << ':' << String::toHexString (temp.combined);
    }

    return getFormattedAddress (addressString);
}

bool IPAddress::operator== (const IPAddress& other) const noexcept    { return compare (other) == 0; }
bool IPAddress::operator!= (const IPAddress& other) const noexcept    { return compare (other) != 0; }
bool IPAddress::operator<  (const IPAddress& other) const noexcept    { return compare (other) <  0; }
bool IPAddress::operator<= (const IPAddress& other) const noexcept    { return compare (other) <= 0; }
bool IPAddress::operator>  (const IPAddress& other) const noexcept    { return compare (other) >  0; }
bool IPAddress::operator>= (const IPAddress& other) const noexcept    { return compare (other) >= 0; }

int IPAddress::compare (const IPAddress& other) const noexcept
{
    if (isIPv6 != other.isIPv6)
    {
        if (isIPv6)
        {
            if (isIPv4MappedAddress (*this))
                return convertIPv4MappedAddressToIPv4 (*this).compare (other);

            return 1;
        }
        else
        {
            if (isIPv4MappedAddress (other))
                return compare (convertIPv4MappedAddressToIPv4 (other));

            return -1;
        }
    }

    for (int i = 0; i < (isIPv6 ? 16 : 4); ++i)
    {
        if (address[i] > other.address[i])
            return 1;
        else if (address[i] < other.address[i])
            return -1;
    }

    return 0;
}

IPAddress IPAddress::any() noexcept                     { return IPAddress(); }
IPAddress IPAddress::broadcast() noexcept               { return IPAddress (255, 255, 255, 255); }
IPAddress IPAddress::local (bool IPv6) noexcept         { return IPv6 ? IPAddress (0, 0, 0, 0, 0, 0, 0, 1)
                                                                      : IPAddress (127, 0, 0, 1); }

String IPAddress::getFormattedAddress (const String& unformattedAddress)
{
    jassert (unformattedAddress.contains (":") && ! unformattedAddress.contains ("::")); // needs to be an unformatted IPv6 address!

    auto portString    = unformattedAddress.fromFirstOccurrenceOf ("]", false, true);
    auto addressString = unformattedAddress.dropLastCharacters (portString.length()).removeCharacters ("[]");

    auto tokens = StringArray::fromTokens (addressString, ":", {});

    int numZeros = 0;
    int numZerosTemp = 0;
    bool isFirst = false;
    bool isLast = false;

    for (int i = 0; i < tokens.size(); ++i)
    {
        auto& t = tokens.getReference (i);

        if (t.getHexValue32() == 0x0000)
        {
            ++numZeros;

            if (i == 0)
                isFirst = true;
            else if (i == tokens.size() - 1 && numZeros > numZerosTemp)
                isLast = true;

            if (t.length() > 1)
                addressString = addressString.replace (String::repeatedString ("0", t.length()), "0");

            if (isFirst && numZerosTemp != 0 && numZeros > numZerosTemp)
                isFirst = false;
        }
        else
        {
            addressString = addressString.replace (t, t.trimCharactersAtStart ("0").toLowerCase());

            if (numZeros > 0)
            {
                if (numZeros > numZerosTemp)
                    numZerosTemp = numZeros;

                numZeros = 0;
            }
        }
    }

    if (numZerosTemp > numZeros)
        numZeros = numZerosTemp;

    if (numZeros > 1)
    {
        if (numZeros == tokens.size())
        {
            addressString = "::,";
        }
        else
        {
            auto zeroString = isFirst ? "0" + String::repeatedString (":0", numZeros - 1)
                                      : String::repeatedString (":0", numZeros);

            addressString = addressString.replaceFirstOccurrenceOf (zeroString, ":");

            if (isLast)
                addressString << ':';
        }
    }

    if (portString.isNotEmpty())
        addressString = "[" + addressString + "]" + portString;

    return addressString;
}

bool IPAddress::isIPv4MappedAddress (const IPAddress& mappedAddress)
{
    if (! mappedAddress.isIPv6)
        return false;

    for (int i = 0; i < 10; ++i)
    {
        if (mappedAddress.address[i] != 0)
            return false;
    }

    if (mappedAddress.address[10] != 255 || mappedAddress.address[11] != 255)
        return false;

    return true;
}

IPAddress IPAddress::convertIPv4MappedAddressToIPv4 (const IPAddress& mappedAddress)
{
    // The address that you're converting needs to be IPv6!
    jassert (mappedAddress.isIPv6);

    if (isIPv4MappedAddress (mappedAddress))
        return { mappedAddress.address[12], mappedAddress.address[13],
                 mappedAddress.address[14], mappedAddress.address[15] };

    return {};
}

IPAddress IPAddress::convertIPv4AddressToIPv4Mapped (const IPAddress& addressToMap)
{
    // The address that you're converting needs to be IPv4!
    jassert (! addressToMap.isIPv6);

    return { 0x0, 0x0, 0x0, 0x0, 0x0, 0xffff,
            static_cast<uint16> ((addressToMap.address[0] << 8) | addressToMap.address[1]),
            static_cast<uint16> ((addressToMap.address[2] << 8) | addressToMap.address[3]) };
}


#if (! JUCE_WINDOWS) && (! JUCE_ANDROID)
static void addAddress (const sockaddr_in* addr_in, Array<IPAddress>& result)
{
    auto addr = addr_in->sin_addr.s_addr;

    if (addr != INADDR_NONE)
        result.addIfNotAlreadyThere (IPAddress (ntohl (addr)));
}

static void addAddress (const sockaddr_in6* addr_in, Array<IPAddress>& result)
{
    in6_addr addr = addr_in->sin6_addr;

    union ByteUnion
    {
        uint16 combined;
        uint8 split[2];
    };

    ByteUnion temp;
    uint16 arr[8];

    for (int i = 0; i < 8; ++i) // Swap bytes from network to host order
    {
        temp.split[0] = addr.s6_addr[i * 2 + 1];
        temp.split[1] = addr.s6_addr[i * 2];

        arr[i] = temp.combined;
    }

    result.addIfNotAlreadyThere (IPAddress (arr));
}

void IPAddress::findAllAddresses (Array<IPAddress>& result, bool includeIPv6)
{
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs (&ifaddr) == -1)
        return;

    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == nullptr)
            continue;

        if      (ifa->ifa_addr->sa_family == AF_INET)                 addAddress ((const sockaddr_in*)  ifa->ifa_addr, result);
        else if (ifa->ifa_addr->sa_family == AF_INET6 && includeIPv6) addAddress ((const sockaddr_in6*) ifa->ifa_addr, result);
    }

    freeifaddrs (ifaddr);
}
#endif

} // namespace juce
