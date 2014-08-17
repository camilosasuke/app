/* 
* libepp-nicbr 1.10 on 2010-14-10 2009 Registro.br
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
* 1. Redistribution of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY REGISTRO.BR ``AS IS AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIE OF FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
* EVENT SHALL REGISTRO.BR BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*/
/* $Id: IpNetworkCreate.cpp 963 2008-12-11 15:31:29Z rafael $ */
#include "StdAfx.H"


namespace libepp 
{

   void IpNetworkCreate::set_xml_template(DomParser * pparser, const char * xml_template)
   {
      StrUtil su(get_app());
      Action::set_xml_template_common(xml_template);
      string_to_string_map to_parse;

      // Ip Range
      string ipRangeVersion = get_command()->get_ipRange().get_version();
      to_parse["ipRange.version"] = "";
      if (ipRangeVersion != "") {
         to_parse["ipRange.version"] = "version='" +
            su.esc_xml_markup(get_command()->get_ipRange().get_version()) + "'";
      }

      to_parse["ipRange.begin"] = 
         su.esc_xml_markup(get_command()->get_ipRange().get_ipBegin());
      to_parse["ipRange.end"] = 
         su.esc_xml_markup(get_command()->get_ipRange().get_ipEnd());

      // Organization
      to_parse["organization"] = 
         su.esc_xml_markup(get_command()->get_organization());

      // Alloc type
      string allocType = get_command()->get_allocType();

      to_parse["allocType"] = "";
      if (allocType != "") {
         to_parse["allocType"] = "<ipnetwork:allocType>" + 
            su.esc_xml_markup(allocType) + "</ipnetwork:allocType>";
      }

      // ASN
      int asn = get_command()->get_asn();

      to_parse["asn"] = "";
      if (asn != 0) {
         to_parse["asn"] = "<ipnetwork:asn>" + gen::str::itoa(asn) +
            "</ipnetwork:asn>";
      }

      // Other contacts
      string_to_string_map contacts = get_command()->get_contacts();
      to_parse["contacts"] = pparser->get_contacts_xml("ipnetwork:contact", contacts);

      // Reverses Dns
      comparable_array<ReverseDns> reversesDns = get_command()->get_reversesDns();
      int reverses = reversesDns.get_size();

      to_parse["reversesDns"] = "";
      for (int i = 0; i < reverses; i++) 
      {
         IpRange ipRange = reversesDns[i].ipRange;

         to_parse["reversesDns"] += "<ipnetwork:reverseDNS>";

         if (ipRange.get_version() != "") 
         {
            to_parse["reversesDns"] += "<ipnetwork:ipRange version='" + su.esc_xml_markup(ipRange.get_version()) + "'>";
         }
         else 
         {
            to_parse["reversesDns"] += "<ipnetwork:ipRange>";
         }

         to_parse["reversesDns"] += "<ipnetwork:startAddress>" + su.esc_xml_markup(ipRange.get_ipBegin()) + "</ipnetwork:startAddress>";
         to_parse["reversesDns"] += "<ipnetwork:endAddress>" + su.esc_xml_markup(ipRange.get_ipEnd()) + "</ipnetwork:endAddress>";
         to_parse["reversesDns"] += "</ipnetwork:ipRange>";

         stringa nameservers = reversesDns[i].nameservers;
         int nameserversSize = nameservers.get_size();

         for (int j = 0; j < nameserversSize; j++) 
         {
            to_parse["reversesDns"] += "<ipnetwork:hostName>" + su.esc_xml_markup(nameservers[j]) + "</ipnetwork:hostName>";
         }

         to_parse["reversesDns"] += "</ipnetwork:reverseDNS>";
      }

      _xml = su.parse(_xml, to_parse, "$(", ")$");
   }

} // namespace libepp
