// Copyright (c) 2013, Jan Winkler <winkler@cs.uni-bremen.de>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Universität Bremen nor the names of its
//       contributors may be used to endorse or promote products derived from
//       this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.


/* \author Jan Winkler */


#ifndef __C_TOC_H__
#define __C_TOC_H__


#include <list>
#include <string>
#include <stdlib.h>

#include "CCrazyRadio.h"
#include "CCRTPPacket.h"


/*! \brief Storage element for logged variable identities */
struct TOCElement {
  /*! \brief The numerical ID of the log element on the copter's
      internal table */
  int nID;
  /*! \brief The (ref) type of the log element */
  int nType;
  /*! \brief The string group name of the log element */
  std::string strGroup;
  /*! \brief The string identifier of the log element */
  std::string strIdentifier;
  bool bIsLogging;
  double dValue;
};


struct LoggingBlock {
  std::string strName;
  int nID;
  double dFrequency;
  std::list<int> lstElementIDs;
};


class CTOC {
 private:
  int m_nPort;
  CCrazyRadio *m_crRadio;
  int m_nItemCount;
  std::list<struct TOCElement> m_lstTOCElements;
  std::list<struct LoggingBlock> m_lstLoggingBlocks;
  
  bool requestInitialItem() {
    return this->requestItem(0, true);
  }
  bool requestItem(int nID, bool bInitial);
  bool requestItem(int nID) {
    return this->requestItem(nID, false);
  }
  bool processItem(CCRTPPacket *crtpItem);
  
  CCRTPPacket *sendAndReceive(CCRTPPacket *crtpSend, int nChannel);
  
 public:
  CTOC(CCrazyRadio *crRadio, int nPort);
  ~CTOC();
  
  bool sendTOCPointerReset();
  bool requestMetaData();
  bool requestItems();
  
  struct TOCElement elementForName(const std::string& strName, bool &bFound) const;
  struct TOCElement elementForID(uint8_t nID, bool &bFound) const;
  int idForName(const std::string& strName) const;
  int typeForName(const std::string& strName) const;

  // For loggable variables only
  bool registerLoggingBlock(const std::string& strName, double dFrequency);
  bool unregisterLoggingBlock(const std::string& strName);
  struct LoggingBlock loggingBlockForName(const std::string& strName, bool &bFound) const;
  struct LoggingBlock loggingBlockForID(uint8_t nID, bool &bFound) const;
  
  bool startLogging(const std::string& strName, const std::string& strBlockName);
  bool stopLogging(const std::string& /*strName*/) {
    return true; // TODO
  }
  bool isLogging(const std::string& /*strName*/) {
    return true; // TODO
  }

  double doubleValue(const std::string& strName) const;
  
  bool enableLogging(const std::string& strBlockName);
  
  void processPackets(std::list<CCRTPPacket*> lstPackets);
  
  int elementIDinBlock(int nBlockID, int nElementIndex) const;
  bool setFloatValueForElementID(int nElementID, float fValue);
  bool addElementToBlock(int nBlockID, int nElementID);
  bool unregisterLoggingBlockID(int nID);
};


#endif /* __C_TOC_H__ */
