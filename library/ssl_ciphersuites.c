/**
 * \file ssl_ciphersuites.c
 *
 * \brief SSL ciphersuites for PolarSSL
 *
 *  Copyright (C) 2006-2013, Brainspark B.V.
 *
 *  This file is part of PolarSSL (http://www.polarssl.org)
 *  Lead Maintainer: Paul Bakker <polarssl_maintainer at polarssl.org>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "polarssl/config.h"

#if defined(POLARSSL_SSL_TLS_C)

#include "polarssl/ssl_ciphersuites.h"
#include "polarssl/ssl.h"

#include <stdlib.h>

/*
 * Ordered from most preferred to least preferred in terms of security.
 */
static const int ciphersuite_preference[] =
{
    /* All AES-256 ephemeral suites */
    TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384,
    TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384,
    TLS_DHE_RSA_WITH_AES_256_CBC_SHA256,
    TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,
    TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384,
    TLS_DHE_RSA_WITH_AES_256_GCM_SHA384,
    TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA,
    TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA,
    TLS_DHE_RSA_WITH_AES_256_CBC_SHA,

    /* All CAMELLIA-256 ephemeral suites */
    TLS_ECDHE_ECDSA_WITH_CAMELLIA_256_CBC_SHA384,
    TLS_ECDHE_RSA_WITH_CAMELLIA_256_CBC_SHA384,
    TLS_DHE_RSA_WITH_CAMELLIA_256_CBC_SHA256,
    TLS_DHE_RSA_WITH_CAMELLIA_256_CBC_SHA,

    /* All AES-128 ephemeral suites */
    TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256,
    TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256,
    TLS_DHE_RSA_WITH_AES_128_CBC_SHA256,
    TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
    TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
    TLS_DHE_RSA_WITH_AES_128_GCM_SHA256,
    TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA,
    TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA,
    TLS_DHE_RSA_WITH_AES_128_CBC_SHA,

    /* All CAMELLIA-128 ephemeral suites */
    TLS_ECDHE_ECDSA_WITH_CAMELLIA_128_CBC_SHA256,
    TLS_ECDHE_RSA_WITH_CAMELLIA_128_CBC_SHA256,
    TLS_DHE_RSA_WITH_CAMELLIA_128_CBC_SHA256,
    TLS_DHE_RSA_WITH_CAMELLIA_128_CBC_SHA,

    /* All remaining > 128-bit ephemeral suites */
    TLS_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA,
    TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA,
    TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA,
    TLS_ECDHE_ECDSA_WITH_RC4_128_SHA,
    TLS_ECDHE_RSA_WITH_RC4_128_SHA,

    /* The PSK ephemeral suites */
    TLS_DHE_PSK_WITH_AES_256_CBC_SHA384,
    TLS_DHE_PSK_WITH_AES_256_CBC_SHA,
    TLS_DHE_PSK_WITH_AES_256_GCM_SHA384,
    TLS_DHE_PSK_WITH_CAMELLIA_256_CBC_SHA384,
    TLS_DHE_PSK_WITH_AES_128_CBC_SHA256,
    TLS_DHE_PSK_WITH_AES_128_CBC_SHA,
    TLS_DHE_PSK_WITH_AES_128_GCM_SHA256,
    TLS_DHE_PSK_WITH_CAMELLIA_128_CBC_SHA256,
    TLS_DHE_PSK_WITH_3DES_EDE_CBC_SHA,
    TLS_DHE_PSK_WITH_RC4_128_SHA,

    /* All AES-256 suites */
    TLS_RSA_WITH_AES_256_CBC_SHA256,
    TLS_RSA_WITH_AES_256_GCM_SHA384,
    TLS_RSA_WITH_AES_256_CBC_SHA,

    /* All CAMELLIA-256 suites */
    TLS_RSA_WITH_CAMELLIA_256_CBC_SHA256,
    TLS_RSA_WITH_CAMELLIA_256_CBC_SHA,

    /* All AES-128 suites */
    TLS_RSA_WITH_AES_128_CBC_SHA256,
    TLS_RSA_WITH_AES_128_GCM_SHA256,
    TLS_RSA_WITH_AES_128_CBC_SHA,

    /* All CAMELLIA-128 suites */
    TLS_RSA_WITH_CAMELLIA_128_CBC_SHA256,
    TLS_RSA_WITH_CAMELLIA_128_CBC_SHA,

    /* All remaining > 128-bit suites */
    TLS_RSA_WITH_3DES_EDE_CBC_SHA,
    TLS_RSA_WITH_RC4_128_SHA,
    TLS_RSA_WITH_RC4_128_MD5,

    /* The RSA PSK suites */
    TLS_RSA_PSK_WITH_AES_256_CBC_SHA384,
    TLS_RSA_PSK_WITH_AES_256_CBC_SHA,
    TLS_RSA_PSK_WITH_AES_256_GCM_SHA384,
    TLS_RSA_PSK_WITH_CAMELLIA_256_CBC_SHA384,
    TLS_RSA_PSK_WITH_AES_128_CBC_SHA256,
    TLS_RSA_PSK_WITH_AES_128_CBC_SHA,
    TLS_RSA_PSK_WITH_AES_128_GCM_SHA256,
    TLS_RSA_PSK_WITH_CAMELLIA_128_CBC_SHA256,
    TLS_RSA_PSK_WITH_3DES_EDE_CBC_SHA,
    TLS_RSA_PSK_WITH_RC4_128_SHA,

    /* The PSK suites */
    TLS_PSK_WITH_AES_256_CBC_SHA384,
    TLS_PSK_WITH_AES_256_CBC_SHA,
    TLS_PSK_WITH_AES_256_GCM_SHA384,
    TLS_PSK_WITH_CAMELLIA_256_CBC_SHA384,
    TLS_PSK_WITH_AES_128_CBC_SHA256,
    TLS_PSK_WITH_AES_128_CBC_SHA,
    TLS_PSK_WITH_AES_128_GCM_SHA256,
    TLS_PSK_WITH_CAMELLIA_128_CBC_SHA256,
    TLS_PSK_WITH_3DES_EDE_CBC_SHA,
    TLS_PSK_WITH_RC4_128_SHA,

    /* Weak or NULL suites */
    TLS_DHE_RSA_WITH_DES_CBC_SHA,
    TLS_RSA_WITH_DES_CBC_SHA,
    TLS_ECDHE_ECDSA_WITH_NULL_SHA,
    TLS_ECDHE_RSA_WITH_NULL_SHA,
    TLS_RSA_WITH_NULL_SHA256,
    TLS_RSA_WITH_NULL_SHA,
    TLS_RSA_WITH_NULL_MD5,
    TLS_PSK_WITH_NULL_SHA384,
    TLS_PSK_WITH_NULL_SHA256,
    TLS_PSK_WITH_NULL_SHA,
    TLS_DHE_PSK_WITH_NULL_SHA384,
    TLS_DHE_PSK_WITH_NULL_SHA256,
    TLS_DHE_PSK_WITH_NULL_SHA,
    TLS_RSA_PSK_WITH_NULL_SHA384,
    TLS_RSA_PSK_WITH_NULL_SHA256,
    TLS_RSA_PSK_WITH_NULL_SHA,

    0
};

#define MAX_CIPHERSUITES    128
static int supported_ciphersuites[MAX_CIPHERSUITES];
static int supported_init = 0;

static const ssl_ciphersuite_t ciphersuite_definitions[] =
{
#if defined(POLARSSL_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED)
#if defined(POLARSSL_AES_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA, "TLS-ECDHE-ECDSA-WITH-AES-128-CBC-SHA",
      POLARSSL_CIPHER_AES_128_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_ECDHE_ECDSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_1,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
    { TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA, "TLS-ECDHE-ECDSA-WITH-AES-256-CBC-SHA",
      POLARSSL_CIPHER_AES_256_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_ECDHE_ECDSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_1,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#if defined(POLARSSL_SHA256_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256, "TLS-ECDHE-ECDSA-WITH-AES-128-CBC-SHA256",
      POLARSSL_CIPHER_AES_128_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_ECDHE_ECDSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#if defined(POLARSSL_GCM_C)
    { TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256, "TLS-ECDHE-ECDSA-WITH-AES-128-GCM-SHA256",
      POLARSSL_CIPHER_AES_128_GCM, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_ECDHE_ECDSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_GCM_C */
#endif /* POLARSSL_SHA256_C */
#if defined(POLARSSL_SHA512_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384, "TLS-ECDHE-ECDSA-WITH-AES-256-CBC-SHA384",
      POLARSSL_CIPHER_AES_256_CBC, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_ECDHE_ECDSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#if defined(POLARSSL_GCM_C)
    { TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384, "TLS-ECDHE-ECDSA-WITH-AES-256-GCM-SHA384",
      POLARSSL_CIPHER_AES_256_GCM, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_ECDHE_ECDSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_GCM_C */
#endif /* POLARSSL_SHA512_C */
#endif /* POLARSSL_AES_C */

#if defined(POLARSSL_CAMELLIA_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
#if defined(POLARSSL_SHA256_C)
    { TLS_ECDHE_ECDSA_WITH_CAMELLIA_128_CBC_SHA256, "TLS-ECDHE-ECDSA-WITH-CAMELLIA-128-CBC-SHA256",
      POLARSSL_CIPHER_CAMELLIA_128_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_ECDHE_ECDSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA256_C */
#if defined(POLARSSL_SHA512_C)
    { TLS_ECDHE_ECDSA_WITH_CAMELLIA_256_CBC_SHA384, "TLS-ECDHE-ECDSA-WITH-CAMELLIA-256-CBC-SHA384",
      POLARSSL_CIPHER_CAMELLIA_256_CBC, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_ECDHE_ECDSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA512_C */
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_CAMELLIA_C */

#if defined(POLARSSL_DES_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA, "TLS-ECDHE-ECDSA-WITH-3DES-EDE-CBC-SHA",
      POLARSSL_CIPHER_DES_EDE3_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_ECDHE_ECDSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_1,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_DES_C */

#if defined(POLARSSL_ARC4_C)
    { TLS_ECDHE_ECDSA_WITH_RC4_128_SHA, "TLS-ECDHE-ECDSA-WITH-RC4-128-SHA",
      POLARSSL_CIPHER_ARC4_128, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_ECDHE_ECDSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_1,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_ARC4_C */

#if defined(POLARSSL_CIPHER_NULL_CIPHER)
    { TLS_ECDHE_ECDSA_WITH_NULL_SHA, "TLS-ECDHE-ECDSA-WITH-NULL-SHA",
      POLARSSL_CIPHER_NULL, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_ECDHE_ECDSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_1,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      POLARSSL_CIPHERSUITE_WEAK },
#endif /* POLARSSL_CIPHER_NULL_CIPHER */
#endif /* POLARSSL_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED */

#if defined(POLARSSL_KEY_EXCHANGE_ECDHE_RSA_ENABLED)
#if defined(POLARSSL_AES_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA, "TLS-ECDHE-RSA-WITH-AES-128-CBC-SHA",
      POLARSSL_CIPHER_AES_128_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_ECDHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_1,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
    { TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA, "TLS-ECDHE-RSA-WITH-AES-256-CBC-SHA",
      POLARSSL_CIPHER_AES_256_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_ECDHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_1,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#if defined(POLARSSL_SHA256_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256, "TLS-ECDHE-RSA-WITH-AES-128-CBC-SHA256",
      POLARSSL_CIPHER_AES_128_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_ECDHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#if defined(POLARSSL_GCM_C)
    { TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256, "TLS-ECDHE-RSA-WITH-AES-128-GCM-SHA256",
      POLARSSL_CIPHER_AES_128_GCM, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_ECDHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_GCM_C */
#endif /* POLARSSL_SHA256_C */
#if defined(POLARSSL_SHA512_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384, "TLS-ECDHE-RSA-WITH-AES-256-CBC-SHA384",
      POLARSSL_CIPHER_AES_256_CBC, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_ECDHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#if defined(POLARSSL_GCM_C)
    { TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384, "TLS-ECDHE-RSA-WITH-AES-256-GCM-SHA384",
      POLARSSL_CIPHER_AES_256_GCM, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_ECDHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_GCM_C */
#endif /* POLARSSL_SHA512_C */
#endif /* POLARSSL_AES_C */

#if defined(POLARSSL_CAMELLIA_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
#if defined(POLARSSL_SHA256_C)
    { TLS_ECDHE_RSA_WITH_CAMELLIA_128_CBC_SHA256, "TLS-ECDHE-RSA-WITH-CAMELLIA-128-CBC-SHA256",
      POLARSSL_CIPHER_CAMELLIA_128_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_ECDHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA256_C */
#if defined(POLARSSL_SHA512_C)
    { TLS_ECDHE_RSA_WITH_CAMELLIA_256_CBC_SHA384, "TLS-ECDHE-RSA-WITH-CAMELLIA-256-CBC-SHA384",
      POLARSSL_CIPHER_CAMELLIA_256_CBC, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_ECDHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA512_C */
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_CAMELLIA_C */

#if defined(POLARSSL_DES_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA, "TLS-ECDHE-RSA-WITH-3DES-EDE-CBC-SHA",
      POLARSSL_CIPHER_DES_EDE3_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_ECDHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_1,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_DES_C */

#if defined(POLARSSL_ARC4_C)
    { TLS_ECDHE_RSA_WITH_RC4_128_SHA, "TLS-ECDHE-RSA-WITH-RC4-128-SHA",
      POLARSSL_CIPHER_ARC4_128, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_ECDHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_1,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_ARC4_C */

#if defined(POLARSSL_CIPHER_NULL_CIPHER)
    { TLS_ECDHE_RSA_WITH_NULL_SHA, "TLS-ECDHE-RSA-WITH-NULL-SHA",
      POLARSSL_CIPHER_NULL, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_ECDHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_1,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      POLARSSL_CIPHERSUITE_WEAK },
#endif /* POLARSSL_CIPHER_NULL_CIPHER */
#endif /* POLARSSL_KEY_EXCHANGE_ECDHE_RSA_ENABLED */

#if defined(POLARSSL_KEY_EXCHANGE_DHE_RSA_ENABLED)
#if defined(POLARSSL_AES_C)
#if defined(POLARSSL_SHA512_C) && defined(POLARSSL_GCM_C)
    { TLS_DHE_RSA_WITH_AES_256_GCM_SHA384, "TLS-DHE-RSA-WITH-AES-256-GCM-SHA384",
      POLARSSL_CIPHER_AES_256_GCM, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_DHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA512_C && POLARSSL_GCM_C */

#if defined(POLARSSL_SHA256_C)
#if defined(POLARSSL_GCM_C)
    { TLS_DHE_RSA_WITH_AES_128_GCM_SHA256, "TLS-DHE-RSA-WITH-AES-128-GCM-SHA256",
      POLARSSL_CIPHER_AES_128_GCM, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_DHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_GCM_C */

#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_DHE_RSA_WITH_AES_128_CBC_SHA256, "TLS-DHE-RSA-WITH-AES-128-CBC-SHA256",
      POLARSSL_CIPHER_AES_128_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_DHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },

    { TLS_DHE_RSA_WITH_AES_256_CBC_SHA256, "TLS-DHE-RSA-WITH-AES-256-CBC-SHA256",
      POLARSSL_CIPHER_AES_256_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_DHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_SHA256_C */

#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_DHE_RSA_WITH_AES_128_CBC_SHA, "TLS-DHE-RSA-WITH-AES-128-CBC-SHA",
      POLARSSL_CIPHER_AES_128_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_DHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },

    { TLS_DHE_RSA_WITH_AES_256_CBC_SHA, "TLS-DHE-RSA-WITH-AES-256-CBC-SHA",
      POLARSSL_CIPHER_AES_256_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_DHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_AES_C */

#if defined(POLARSSL_CAMELLIA_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
#if defined(POLARSSL_SHA256_C)
    { TLS_DHE_RSA_WITH_CAMELLIA_128_CBC_SHA256, "TLS-DHE-RSA-WITH-CAMELLIA-128-CBC-SHA256",
      POLARSSL_CIPHER_CAMELLIA_128_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_DHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },

    { TLS_DHE_RSA_WITH_CAMELLIA_256_CBC_SHA256, "TLS-DHE-RSA-WITH-CAMELLIA-256-CBC-SHA256",
      POLARSSL_CIPHER_CAMELLIA_256_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_DHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA256_C */

    { TLS_DHE_RSA_WITH_CAMELLIA_128_CBC_SHA, "TLS-DHE-RSA-WITH-CAMELLIA-128-CBC-SHA",
      POLARSSL_CIPHER_CAMELLIA_128_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_DHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },

    { TLS_DHE_RSA_WITH_CAMELLIA_256_CBC_SHA, "TLS-DHE-RSA-WITH-CAMELLIA-256-CBC-SHA",
      POLARSSL_CIPHER_CAMELLIA_256_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_DHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_CAMELLIA_C */

#if defined(POLARSSL_DES_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA, "TLS-DHE-RSA-WITH-3DES-EDE-CBC-SHA",
      POLARSSL_CIPHER_DES_EDE3_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_DHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_DES_C */
#endif /* POLARSSL_KEY_EXCHANGE_DHE_RSA_ENABLED */

#if defined(POLARSSL_KEY_EXCHANGE_RSA_ENABLED)
#if defined(POLARSSL_AES_C)
#if defined(POLARSSL_SHA512_C) && defined(POLARSSL_GCM_C)
    { TLS_RSA_WITH_AES_256_GCM_SHA384, "TLS-RSA-WITH-AES-256-GCM-SHA384",
      POLARSSL_CIPHER_AES_256_GCM, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA512_C && POLARSSL_GCM_C */

#if defined(POLARSSL_SHA256_C)
#if defined(POLARSSL_GCM_C)
    { TLS_RSA_WITH_AES_128_GCM_SHA256, "TLS-RSA-WITH-AES-128-GCM-SHA256",
      POLARSSL_CIPHER_AES_128_GCM, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_GCM_C */

#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_RSA_WITH_AES_128_CBC_SHA256, "TLS-RSA-WITH-AES-128-CBC-SHA256",
      POLARSSL_CIPHER_AES_128_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },

    { TLS_RSA_WITH_AES_256_CBC_SHA256, "TLS-RSA-WITH-AES-256-CBC-SHA256",
      POLARSSL_CIPHER_AES_256_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_SHA256_C */

#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_RSA_WITH_AES_128_CBC_SHA, "TLS-RSA-WITH-AES-128-CBC-SHA",
      POLARSSL_CIPHER_AES_128_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },

    { TLS_RSA_WITH_AES_256_CBC_SHA, "TLS-RSA-WITH-AES-256-CBC-SHA",
      POLARSSL_CIPHER_AES_256_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_AES_C */

#if defined(POLARSSL_CAMELLIA_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
#if defined(POLARSSL_SHA256_C)
    { TLS_RSA_WITH_CAMELLIA_128_CBC_SHA256, "TLS-RSA-WITH-CAMELLIA-128-CBC-SHA256",
      POLARSSL_CIPHER_CAMELLIA_128_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },

    { TLS_RSA_WITH_CAMELLIA_256_CBC_SHA256, "TLS-RSA-WITH-CAMELLIA-256-CBC-SHA256",
      POLARSSL_CIPHER_CAMELLIA_256_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA256_C */

    { TLS_RSA_WITH_CAMELLIA_128_CBC_SHA, "TLS-RSA-WITH-CAMELLIA-128-CBC-SHA",
      POLARSSL_CIPHER_CAMELLIA_128_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },

    { TLS_RSA_WITH_CAMELLIA_256_CBC_SHA, "TLS-RSA-WITH-CAMELLIA-256-CBC-SHA",
      POLARSSL_CIPHER_CAMELLIA_256_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_CAMELLIA_C */

#if defined(POLARSSL_DES_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_RSA_WITH_3DES_EDE_CBC_SHA, "TLS-RSA-WITH-3DES-EDE-CBC-SHA",
      POLARSSL_CIPHER_DES_EDE3_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_DES_C */

#if defined(POLARSSL_ARC4_C)
    { TLS_RSA_WITH_RC4_128_MD5, "TLS-RSA-WITH-RC4-128-MD5",
      POLARSSL_CIPHER_ARC4_128, POLARSSL_MD_MD5, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },

    { TLS_RSA_WITH_RC4_128_SHA, "TLS-RSA-WITH-RC4-128-SHA",
      POLARSSL_CIPHER_ARC4_128, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_ARC4_C */
#endif /* POLARSSL_KEY_EXCHANGE_RSA_ENABLED */

#if defined(POLARSSL_KEY_EXCHANGE_PSK_ENABLED)
#if defined(POLARSSL_AES_C)
#if defined(POLARSSL_GCM_C)
#if defined(POLARSSL_SHA256_C)
    { TLS_PSK_WITH_AES_128_GCM_SHA256, "TLS-PSK-WITH-AES-128-GCM-SHA256",
      POLARSSL_CIPHER_AES_128_GCM, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA256_C */

#if defined(POLARSSL_SHA512_C)
    { TLS_PSK_WITH_AES_256_GCM_SHA384, "TLS-PSK-WITH-AES-256-GCM-SHA384",
      POLARSSL_CIPHER_AES_256_GCM, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA512_C */
#endif /* POLARSSL_GCM_C */

#if defined(POLARSSL_CIPHER_MODE_CBC)
#if defined(POLARSSL_SHA256_C)
    { TLS_PSK_WITH_AES_128_CBC_SHA256, "TLS-PSK-WITH-AES-128-CBC-SHA256",
      POLARSSL_CIPHER_AES_128_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA256_C */

#if defined(POLARSSL_SHA512_C)
    { TLS_PSK_WITH_AES_256_CBC_SHA384, "TLS-PSK-WITH-AES-256-CBC-SHA384",
      POLARSSL_CIPHER_AES_256_CBC, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA512_C */

    { TLS_PSK_WITH_AES_128_CBC_SHA, "TLS-PSK-WITH-AES-128-CBC-SHA",
      POLARSSL_CIPHER_AES_128_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },

    { TLS_PSK_WITH_AES_256_CBC_SHA, "TLS-PSK-WITH-AES-256-CBC-SHA",
      POLARSSL_CIPHER_AES_256_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_AES_C */

#if defined(POLARSSL_CAMELLIA_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
#if defined(POLARSSL_SHA256_C)
    { TLS_PSK_WITH_CAMELLIA_128_CBC_SHA256, "TLS-PSK-WITH-CAMELLIA-128-CBC-SHA256",
      POLARSSL_CIPHER_CAMELLIA_128_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA256_C */

#if defined(POLARSSL_SHA512_C)
    { TLS_PSK_WITH_CAMELLIA_256_CBC_SHA384, "TLS-PSK-WITH-CAMELLIA-256-CBC-SHA384",
      POLARSSL_CIPHER_CAMELLIA_256_CBC, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA512_C */
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_CAMELLIA_C */

#if defined(POLARSSL_DES_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_PSK_WITH_3DES_EDE_CBC_SHA, "TLS-PSK-WITH-3DES-EDE-CBC-SHA",
      POLARSSL_CIPHER_DES_EDE3_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_DES_C */

#if defined(POLARSSL_ARC4_C)
    { TLS_PSK_WITH_RC4_128_SHA, "TLS-PSK-WITH-RC4-128-SHA",
      POLARSSL_CIPHER_ARC4_128, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_ARC4_C */
#endif /* POLARSSL_KEY_EXCHANGE_PSK_ENABLED */

#if defined(POLARSSL_KEY_EXCHANGE_DHE_PSK_ENABLED)
#if defined(POLARSSL_AES_C)
#if defined(POLARSSL_GCM_C)
#if defined(POLARSSL_SHA256_C)
    { TLS_DHE_PSK_WITH_AES_128_GCM_SHA256, "TLS-DHE-PSK-WITH-AES-128-GCM-SHA256",
      POLARSSL_CIPHER_AES_128_GCM, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_DHE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA256_C */

#if defined(POLARSSL_SHA512_C)
    { TLS_DHE_PSK_WITH_AES_256_GCM_SHA384, "TLS-DHE-PSK-WITH-AES-256-GCM-SHA384",
      POLARSSL_CIPHER_AES_256_GCM, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_DHE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA512_C */
#endif /* POLARSSL_GCM_C */

#if defined(POLARSSL_CIPHER_MODE_CBC)
#if defined(POLARSSL_SHA256_C)
    { TLS_DHE_PSK_WITH_AES_128_CBC_SHA256, "TLS-DHE-PSK-WITH-AES-128-CBC-SHA256",
      POLARSSL_CIPHER_AES_128_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_DHE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA256_C */

#if defined(POLARSSL_SHA512_C)
    { TLS_DHE_PSK_WITH_AES_256_CBC_SHA384, "TLS-DHE-PSK-WITH-AES-256-CBC-SHA384",
      POLARSSL_CIPHER_AES_256_CBC, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_DHE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA512_C */

    { TLS_DHE_PSK_WITH_AES_128_CBC_SHA, "TLS-DHE-PSK-WITH-AES-128-CBC-SHA",
      POLARSSL_CIPHER_AES_128_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_DHE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },

    { TLS_DHE_PSK_WITH_AES_256_CBC_SHA, "TLS-DHE-PSK-WITH-AES-256-CBC-SHA",
      POLARSSL_CIPHER_AES_256_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_DHE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_AES_C */

#if defined(POLARSSL_CAMELLIA_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
#if defined(POLARSSL_SHA256_C)
    { TLS_DHE_PSK_WITH_CAMELLIA_128_CBC_SHA256, "TLS-DHE-PSK-WITH-CAMELLIA-128-CBC-SHA256",
      POLARSSL_CIPHER_CAMELLIA_128_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_DHE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA256_C */

#if defined(POLARSSL_SHA512_C)
    { TLS_DHE_PSK_WITH_CAMELLIA_256_CBC_SHA384, "TLS-DHE-PSK-WITH-CAMELLIA-256-CBC-SHA384",
      POLARSSL_CIPHER_CAMELLIA_256_CBC, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_DHE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA512_C */
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_CAMELLIA_C */

#if defined(POLARSSL_DES_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_DHE_PSK_WITH_3DES_EDE_CBC_SHA, "TLS-DHE-PSK-WITH-3DES-EDE-CBC-SHA",
      POLARSSL_CIPHER_DES_EDE3_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_DHE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_DES_C */

#if defined(POLARSSL_ARC4_C)
    { TLS_DHE_PSK_WITH_RC4_128_SHA, "TLS-DHE-PSK-WITH-RC4-128-SHA",
      POLARSSL_CIPHER_ARC4_128, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_DHE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_ARC4_C */
#endif /* POLARSSL_KEY_EXCHANGE_DHE_PSK_ENABLED */

#if defined(POLARSSL_KEY_EXCHANGE_RSA_PSK_ENABLED)
#if defined(POLARSSL_AES_C)
#if defined(POLARSSL_GCM_C)
#if defined(POLARSSL_SHA256_C)
    { TLS_RSA_PSK_WITH_AES_128_GCM_SHA256, "TLS-RSA-PSK-WITH-AES-128-GCM-SHA256",
      POLARSSL_CIPHER_AES_128_GCM, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_RSA_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA256_C */

#if defined(POLARSSL_SHA512_C)
    { TLS_RSA_PSK_WITH_AES_256_GCM_SHA384, "TLS-RSA-PSK-WITH-AES-256-GCM-SHA384",
      POLARSSL_CIPHER_AES_256_GCM, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_RSA_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA512_C */
#endif /* POLARSSL_GCM_C */

#if defined(POLARSSL_CIPHER_MODE_CBC)
#if defined(POLARSSL_SHA256_C)
    { TLS_RSA_PSK_WITH_AES_128_CBC_SHA256, "TLS-RSA-PSK-WITH-AES-128-CBC-SHA256",
      POLARSSL_CIPHER_AES_128_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_RSA_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA256_C */

#if defined(POLARSSL_SHA512_C)
    { TLS_RSA_PSK_WITH_AES_256_CBC_SHA384, "TLS-RSA-PSK-WITH-AES-256-CBC-SHA384",
      POLARSSL_CIPHER_AES_256_CBC, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_RSA_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA512_C */

    { TLS_RSA_PSK_WITH_AES_128_CBC_SHA, "TLS-RSA-PSK-WITH-AES-128-CBC-SHA",
      POLARSSL_CIPHER_AES_128_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_RSA_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },

    { TLS_RSA_PSK_WITH_AES_256_CBC_SHA, "TLS-RSA-PSK-WITH-AES-256-CBC-SHA",
      POLARSSL_CIPHER_AES_256_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_RSA_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_AES_C */

#if defined(POLARSSL_CAMELLIA_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
#if defined(POLARSSL_SHA256_C)
    { TLS_RSA_PSK_WITH_CAMELLIA_128_CBC_SHA256, "TLS-RSA-PSK-WITH-CAMELLIA-128-CBC-SHA256",
      POLARSSL_CIPHER_CAMELLIA_128_CBC, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_RSA_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA256_C */

#if defined(POLARSSL_SHA512_C)
    { TLS_RSA_PSK_WITH_CAMELLIA_256_CBC_SHA384, "TLS-RSA-PSK-WITH-CAMELLIA-256-CBC-SHA384",
      POLARSSL_CIPHER_CAMELLIA_256_CBC, POLARSSL_MD_SHA384, POLARSSL_KEY_EXCHANGE_RSA_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_SHA512_C */
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_CAMELLIA_C */

#if defined(POLARSSL_DES_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
    { TLS_RSA_PSK_WITH_3DES_EDE_CBC_SHA, "TLS-RSA-PSK-WITH-3DES-EDE-CBC-SHA",
      POLARSSL_CIPHER_DES_EDE3_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_RSA_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_DES_C */

#if defined(POLARSSL_ARC4_C)
    { TLS_RSA_PSK_WITH_RC4_128_SHA, "TLS-RSA-PSK-WITH-RC4-128-SHA",
      POLARSSL_CIPHER_ARC4_128, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_RSA_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      0 },
#endif /* POLARSSL_ARC4_C */
#endif /* POLARSSL_KEY_EXCHANGE_RSA_PSK_ENABLED */

#if defined(POLARSSL_ENABLE_WEAK_CIPHERSUITES)
#if defined(POLARSSL_CIPHER_NULL_CIPHER)
#if defined(POLARSSL_KEY_EXCHANGE_RSA_ENABLED)
    { TLS_RSA_WITH_NULL_MD5, "TLS-RSA-WITH-NULL-MD5",
      POLARSSL_CIPHER_NULL, POLARSSL_MD_MD5, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      POLARSSL_CIPHERSUITE_WEAK },

    { TLS_RSA_WITH_NULL_SHA, "TLS-RSA-WITH-NULL-SHA",
      POLARSSL_CIPHER_NULL, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      POLARSSL_CIPHERSUITE_WEAK },

    { TLS_RSA_WITH_NULL_SHA256, "TLS-RSA-WITH-NULL-SHA256",
      POLARSSL_CIPHER_NULL, POLARSSL_MD_SHA256, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      POLARSSL_CIPHERSUITE_WEAK },
#endif /* POLARSSL_KEY_EXCHANGE_RSA_ENABLED */

#if defined(POLARSSL_KEY_EXCHANGE_PSK_ENABLED)
    { TLS_PSK_WITH_NULL_SHA, "TLS-PSK-WITH-NULL-SHA",
      POLARSSL_CIPHER_NULL, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      POLARSSL_CIPHERSUITE_WEAK },
#endif /* POLARSSL_KEY_EXCHANGE_PSK_ENABLED */

#if defined(POLARSSL_KEY_EXCHANGE_DHE_PSK_ENABLED)
    { TLS_DHE_PSK_WITH_NULL_SHA, "TLS-DHE-PSK-WITH-NULL-SHA",
      POLARSSL_CIPHER_NULL, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_DHE_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      POLARSSL_CIPHERSUITE_WEAK },
#endif /* POLARSSL_KEY_EXCHANGE_DHE_PSK_ENABLED */

#if defined(POLARSSL_KEY_EXCHANGE_RSA_PSK_ENABLED)
    { TLS_RSA_PSK_WITH_NULL_SHA, "TLS-RSA-PSK-WITH-NULL-SHA",
      POLARSSL_CIPHER_NULL, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_RSA_PSK,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      POLARSSL_CIPHERSUITE_WEAK },
#endif /* POLARSSL_KEY_EXCHANGE_RSA_PSK_ENABLED */
#endif /* POLARSSL_CIPHER_NULL_CIPHER */

#if defined(POLARSSL_DES_C)
#if defined(POLARSSL_CIPHER_MODE_CBC)
#if defined(POLARSSL_KEY_EXCHANGE_DHE_RSA_ENABLED)
    { TLS_DHE_RSA_WITH_DES_CBC_SHA, "TLS-DHE-RSA-WITH-DES-CBC-SHA",
      POLARSSL_CIPHER_DES_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_DHE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      POLARSSL_CIPHERSUITE_WEAK },
#endif /* POLARSSL_KEY_EXCHANGE_DHE_RSA_ENABLED */

#if defined(POLARSSL_KEY_EXCHANGE_RSA_ENABLED)
    { TLS_RSA_WITH_DES_CBC_SHA, "TLS-RSA-WITH-DES-CBC-SHA",
      POLARSSL_CIPHER_DES_CBC, POLARSSL_MD_SHA1, POLARSSL_KEY_EXCHANGE_RSA,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_0,
      SSL_MAJOR_VERSION_3, SSL_MINOR_VERSION_3,
      POLARSSL_CIPHERSUITE_WEAK },
#endif /* POLARSSL_KEY_EXCHANGE_RSA_ENABLED */
#endif /* POLARSSL_CIPHER_MODE_CBC */
#endif /* POLARSSL_DES_C */
#endif /* POLARSSL_ENABLE_WEAK_CIPHERSUITES */

    { 0, "", 0, 0, 0, 0, 0, 0, 0, 0 }
};

const int *ssl_list_ciphersuites( void )
{
    /*
     * On initial call filter out all ciphersuites not supported by current
     * build based on presence in the ciphersuite_definitions.
     */
    if( supported_init == 0 )
    {
        const int *p = ciphersuite_preference;
        int *q = supported_ciphersuites;
        size_t i;
        size_t max = sizeof(supported_ciphersuites) / sizeof(int);

        for( i = 0; i < max - 1 && p[i] != 0; i++ )
        {
            if( ssl_ciphersuite_from_id( p[i] ) != NULL )
                *(q++) = p[i];
        }
        *q = 0;

        supported_init = 1;
    }

    return supported_ciphersuites;
};

const ssl_ciphersuite_t *ssl_ciphersuite_from_string( const char *ciphersuite_name )
{
    const ssl_ciphersuite_t *cur = ciphersuite_definitions;

    if( NULL == ciphersuite_name )
        return( NULL );

    while( cur->id != 0 )
    {
        if( 0 == strcasecmp( cur->name, ciphersuite_name ) )
            return( cur );

        cur++;
    }

    return( NULL );
}

const ssl_ciphersuite_t *ssl_ciphersuite_from_id( int ciphersuite )
{
    const ssl_ciphersuite_t *cur = ciphersuite_definitions;

    while( cur->id != 0 )
    {
        if( cur->id == ciphersuite )
            return( cur );

        cur++;
    }

    return( NULL );
}

const char *ssl_get_ciphersuite_name( const int ciphersuite_id )
{
    const ssl_ciphersuite_t *cur;

    cur = ssl_ciphersuite_from_id( ciphersuite_id );

    if( cur == NULL )
        return( "unknown" );

    return( cur->name );
}

int ssl_get_ciphersuite_id( const char *ciphersuite_name )
{
    const ssl_ciphersuite_t *cur;

    cur = ssl_ciphersuite_from_string( ciphersuite_name );

    if( cur == NULL )
        return( 0 );

    return( cur->id );
}

pk_type_t ssl_get_ciphersuite_sig_pk_alg( const ssl_ciphersuite_t *info )
{
    switch( info->key_exchange )
    {
        case POLARSSL_KEY_EXCHANGE_RSA:
        case POLARSSL_KEY_EXCHANGE_DHE_RSA:
        case POLARSSL_KEY_EXCHANGE_ECDHE_RSA:
        case POLARSSL_KEY_EXCHANGE_RSA_PSK:
            return( POLARSSL_PK_RSA );

        case POLARSSL_KEY_EXCHANGE_ECDHE_ECDSA:
            return( POLARSSL_PK_ECDSA );

        default:
            return( POLARSSL_PK_NONE );
    }
}

int ssl_ciphersuite_uses_ec( const ssl_ciphersuite_t *info )
{
    switch( info->key_exchange )
    {
        case POLARSSL_KEY_EXCHANGE_ECDHE_RSA:
        case POLARSSL_KEY_EXCHANGE_ECDHE_ECDSA:
            return( 1 );

        default:
            return( 0 );
    }
}

#endif
