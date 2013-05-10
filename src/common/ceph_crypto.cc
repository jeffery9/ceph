// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2010-2011 Dreamhost
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software
 * Foundation.  See file COPYING.
 *
 */

#include "common/config.h"
#include "common/ceph_context.h"
#include "ceph_crypto.h"
#include "auth/Crypto.h"

#include <pthread.h>
#include <stdlib.h>


#ifdef USE_CRYPTOPP
void ceph::crypto::init(CephContext *cct)
{
}

void ceph::crypto::shutdown()
{
}

// nothing
ceph::crypto::HMACSHA1::~HMACSHA1()
{
}

#elif USE_NSS

void ceph::crypto::init(CephContext *cct)
{
  SECStatus s;
  cout << "cct is " << cct << std::endl;
  cout << "cct conf is " << (cct ? cct->_conf : NULL) << std::endl;
  if (cct->_conf->nss_db_path.empty()) {
    s = NSS_NoDB_Init(NULL);
  } else {
    s = NSS_Init(cct->_conf->nss_db_path.c_str());
  }
  assert(s == SECSuccess);
}

void ceph::crypto::shutdown()
{
  SECStatus s;
  s = NSS_Shutdown();
  assert(s == SECSuccess);
}

ceph::crypto::HMACSHA1::~HMACSHA1()
{
  PK11_DestroyContext(ctx, PR_TRUE);
  PK11_FreeSymKey(symkey);
  PK11_FreeSlot(slot);
}

#else
# error "No supported crypto implementation found."
#endif
