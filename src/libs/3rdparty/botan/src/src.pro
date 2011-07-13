TEMPLATE = lib
TARGET = Botan

CONFIG += dll

PRECOMPILED_HEADER = ../../precompiled_headers/botan_pch.h

include(../../../../qtcreatorlibrary.pri)

DEPENDPATH += .
INCLUDEPATH += $$PWD $$PWD/../build $$PWD/../build/botan
unix:QMAKE_CXXFLAGS_DEBUG += -O2

win32 {
    win32-msvc* {
        QMAKE_CXXFLAGS += -wd4251 -wd4290 -wd4250
        DEFINES += BOTAN_DLL=__declspec(dllexport)
    } else {
        QMAKE_CFLAGS += -fpermissive -finline-functions -Wno-long-long
        QMAKE_CXXFLAGS += -fpermissive -finline-functions -Wno-long-long
    }
    LIBS += -ladvapi32 -luser32
}

unix:*-gcc* {
    QMAKE_CFLAGS += -fPIC -ansi -fpermissive -finline-functions -Wno-long-long
    QMAKE_CXXFLAGS += -fPIC -ansi -fpermissive -finline-functions -Wno-long-long
    QMAKE_CXXFLAGS_HIDESYMS -= -fvisibility-inlines-hidden # for ubuntu 7.04
}

os2 {
    DEFINES += BOTAN_DLL=__declspec(dllexport)
    QMAKE_CFLAGS += -ansi -fpermissive -finline-functions -Wno-long-long
    QMAKE_CXXFLAGS += -ansi -fpermissive -finline-functions -Wno-long-long
}

# Input
HEADERS += algo_factory/algo_cache.h \
        algo_factory/algo_factory.h \
        alloc/allocate.h \
        alloc/mem_pool/mem_pool.h \
        alloc/secmem.h \
        alloc/system_alloc/defalloc.h \
        asn1/alg_id.h \
        asn1/asn1_int.h \
        asn1/asn1_obj.h \
        asn1/asn1_oid.h \
        asn1/ber_dec.h \
        asn1/der_enc.h \
        benchmark/benchmark.h \
        block/aes/aes.h \
        block/block_cipher.h \
        block/blowfish/blowfish.h \
        block/cast/cast128.h \
        block/cast/cast256.h \
        block/des/des.h \
        block/des/desx.h \
        block/gost_28147/gost_28147.h \
        block/idea/idea.h \
        block/kasumi/kasumi.h \
        block/lion/lion.h \
        block/lubyrack/lubyrack.h \
        block/mars/mars.h \
        block/misty1/misty1.h \
        block/noekeon/noekeon.h \
        block/rc2/rc2.h \
        block/rc5/rc5.h \
        block/rc6/rc6.h \
        block/safer/safer_sk.h \
        block/seed/seed.h \
        block/serpent/serpent.h \
        block/skipjack/skipjack.h \
        block/square/square.h \
        block/tea/tea.h \
        block/twofish/twofish.h \
        block/xtea/xtea.h \
        cert/x509/certstor.h \
        cert/x509/crl_ent.h \
        cert/x509/pkcs10.h \
        cert/x509/x509_ca.h \
        cert/x509/x509_crl.h \
        cert/x509/x509_ext.h \
        cert/x509/x509_obj.h \
        cert/x509/x509cert.h \
        cert/x509/x509find.h \
        cert/x509/x509self.h \
        cert/x509/x509stor.h \
        checksum/adler32/adler32.h \
        checksum/crc24/crc24.h \
        checksum/crc32/crc32.h \
        cms/cms_dec.h \
        cms/cms_enc.h \
        codec/base64/base64.h \
        codec/hex/hex.h \
        codec/openpgp/openpgp.h \
        codec/pem/pem.h \
        cryptobox/cryptobox.h \
        engine/def_engine/def_eng.h \
        engine/engine.h \
        entropy/entropy_src.h \
        filters/basefilt.h \
        filters/buf_filt.h \
        filters/data_snk.h \
        filters/data_src.h \
        filters/filter.h \
        filters/filters.h \
        filters/out_buf.h \
        filters/pbe.h \
        filters/pipe.h \
        filters/secqueue.h \
        hash/fork256/fork256.h \
        hash/gost_3411/gost_3411.h \
        hash/has160/has160.h \
        hash/hash.h \
        hash/md2/md2.h \
        hash/md4/md4.h \
        hash/md5/md5.h \
        hash/mdx_hash/mdx_hash.h \
        hash/par_hash/par_hash.h \
        hash/rmd128/rmd128.h \
        hash/rmd160/rmd160.h \
        hash/sha1/sha160.h \
        hash/sha2/sha2_32.h \
        hash/sha2/sha2_64.h \
        hash/skein/skein_512.h \
        hash/tiger/tiger.h \
        hash/whirlpool/whrlpool.h \
        kdf/kdf.h \
        kdf/kdf1/kdf1.h \
        kdf/kdf2/kdf2.h \
        kdf/mgf1/mgf1.h \
        kdf/ssl_prf/prf_ssl3.h \
        kdf/tls_prf/prf_tls.h \
        kdf/x942_prf/prf_x942.h \
        libstate/botan.h \
        libstate/init.h \
        libstate/libstate.h \
        libstate/look_pk.h \
        libstate/lookup.h \
        libstate/oid_lookup/oids.h \
        libstate/pk_engine.h \
        libstate/scan_name.h \
        mac/cbc_mac/cbc_mac.h \
        mac/cmac/cmac.h \
        mac/hmac/hmac.h \
        mac/mac.h \
        mac/ssl3mac/ssl3_mac.h \
        mac/x919_mac/x919_mac.h \
        math/bigint/bigint.h \
        math/bigint/divide.h \
        math/bigint/mp_core.h \
        math/bigint/mp_generic/mp_asm.h \
        math/bigint/mp_generic/mp_asmi.h \
        math/bigint/mp_types.h \
        math/numbertheory/blinding.h \
        math/numbertheory/def_powm.h \
        math/numbertheory/numthry.h \
        math/numbertheory/pow_mod.h \
        math/numbertheory/reducer.h \
        modes/cbc/cbc.h \
        modes/cfb/cfb.h \
        modes/ctr/ctr.h \
        modes/cts/cts.h \
        modes/eax/eax.h \
        modes/ecb/ecb.h \
        modes/mode_pad/mode_pad.h \
        modes/modebase.h \
        modes/ofb/ofb.h \
        modes/xts/xts.h \
        mutex/mutex.h \
        mutex/noop_mutex/mux_noop.h \
        pbe/get_pbe.h \
        pbe/pbes1/pbes1.h \
        pbe/pbes2/pbes2.h \
        pk_pad/eme.h \
        pk_pad/eme1/eme1.h \
        pk_pad/eme_pkcs/eme_pkcs.h \
        pk_pad/emsa.h \
        pk_pad/emsa1/emsa1.h \
        pk_pad/emsa1_bsi/emsa1_bsi.h \
        pk_pad/emsa2/emsa2.h \
        pk_pad/emsa3/emsa3.h \
        pk_pad/emsa4/emsa4.h \
        pk_pad/emsa_raw/emsa_raw.h \
        pk_pad/hash_id/hash_id.h \
        pubkey/dh/dh.h \
        pubkey/dh/dh_core.h \
        pubkey/dh/dh_op.h \
        pubkey/dl_algo/dl_algo.h \
        pubkey/dl_group/dl_group.h \
        pubkey/dlies/dlies.h \
        pubkey/dsa/dsa.h \
        pubkey/dsa/dsa_core.h \
        pubkey/dsa/dsa_op.h \
        pubkey/elgamal/elg_core.h \
        pubkey/elgamal/elg_op.h \
        pubkey/elgamal/elgamal.h \
        pubkey/if_algo/if_algo.h \
        pubkey/if_algo/if_core.h \
        pubkey/if_algo/if_op.h \
        pubkey/keypair/keypair.h \
        pubkey/nr/nr.h \
        pubkey/nr/nr_core.h \
        pubkey/nr/nr_op.h \
        pubkey/pk_algs.h \
        pubkey/pk_codecs/pkcs8.h \
        pubkey/pk_codecs/x509_key.h \
        pubkey/pk_filts.h \
        pubkey/pk_keys.h \
        pubkey/pubkey.h \
        pubkey/pubkey_enums.h \
        pubkey/rsa/rsa.h \
        pubkey/rw/rw.h \
        rng/auto_rng/auto_rng.h \
        rng/hmac_rng/hmac_rng.h \
        rng/randpool/randpool.h \
        rng/rng.h \
        rng/x931_rng/x931_rng.h \
        s2k/pbkdf1/pbkdf1.h \
        s2k/pbkdf2/pbkdf2.h \
        s2k/pgps2k/pgp_s2k.h \
        s2k/s2k.h \
        selftest/selftest.h \
        stream/arc4/arc4.h \
        stream/salsa20/salsa20.h \
        stream/stream_cipher.h \
        stream/turing/turing.h \
        stream/wid_wake/wid_wake.h \
        sym_algo/sym_algo.h \
        sym_algo/symkey.h \
        timer/timer.h \
        utils/bit_ops.h \
        utils/bswap.h \
        utils/buf_comp/buf_comp.h \
        utils/charset.h \
        utils/datastor/datastor.h \
        utils/exceptn.h \
        utils/loadstor.h \
        utils/mem_ops.h \
        utils/parsing.h \
        utils/rotate.h \
        utils/stl_util.h \
        utils/types.h \
        utils/ui.h \
        utils/util.h \
        utils/version.h \
        utils/xor_buf.h \
    utils/sharedpointer.h

win32 {
    HEADERS += entropy/cryptoapi_rng/es_capi.h \
        entropy/win32_stats/es_win32.h \
        mutex/win32_crit_section/mux_win32.h \
        timer/win32_query_perf_ctr/tm_win32.h
}

unix {
    HEADERS += alloc/alloc_mmap/mmap_mem.h \
           cert/cvc/cvc_ado.h \
           cert/cvc/cvc_ca.h \
           cert/cvc/cvc_cert.h \
           cert/cvc/cvc_gen_cert.h \
           cert/cvc/cvc_key.h \
           cert/cvc/cvc_req.h \
           cert/cvc/cvc_self.h \
           cert/cvc/eac_asn_obj.h \
           cert/cvc/eac_obj.h \
           cert/cvc/ecdsa_sig.h \
           cert/cvc/freestore.h \
           cert/cvc/signed_obj.h \
           entropy/dev_random/es_dev.h \
           entropy/egd/es_egd.h \
           entropy/proc_walk/es_ftw.h \
           entropy/unix_procs/es_unix.h \
           entropy/unix_procs/unix_cmd.h \
           filters/fd_unix/fd_unix.h \
           math/gfpmath/curve_gfp.h \
           math/gfpmath/gfp_element.h \
           math/gfpmath/gfp_modulus.h \
           math/gfpmath/point_gfp.h \
           mutex/pthreads/mux_pthr.h \
           pubkey/ec_dompar/ec_dompar.h \
           pubkey/ecc_key/ecc_key.h \
           pubkey/ecdsa/ecdsa.h \
           pubkey/ecdsa/ecdsa_core.h \
           pubkey/ecdsa/ecdsa_op.h \
           pubkey/eckaeg/eckaeg.h \
           pubkey/eckaeg/eckaeg_core.h \
           pubkey/eckaeg/eckaeg_op.h \
           timer/gettimeofday/tm_unix.h
}

linux*-g++* {
    HEADERS += timer/posix_rt/tm_posix.h
}

os2 {
    HEADERS += \
           cert/cvc/cvc_ado.h \
           cert/cvc/cvc_ca.h \
           cert/cvc/cvc_cert.h \
           cert/cvc/cvc_gen_cert.h \
           cert/cvc/cvc_key.h \
           cert/cvc/cvc_req.h \
           cert/cvc/cvc_self.h \
           cert/cvc/eac_asn_obj.h \
           cert/cvc/eac_obj.h \
           cert/cvc/ecdsa_sig.h \
           cert/cvc/freestore.h \
           cert/cvc/signed_obj.h \
           entropy/egd/es_egd.h \
           entropy/dev_random/es_dev.h \
           entropy/proc_walk/es_ftw.h \
           entropy/os2_stats/es_os2.h \
           filters/fd_unix/fd_unix.h \
           math/gfpmath/curve_gfp.h \
           math/gfpmath/gfp_element.h \
           math/gfpmath/gfp_modulus.h \
           math/gfpmath/point_gfp.h \
           mutex/qt_mutex/mux_qt.h \
           pubkey/ec_dompar/ec_dompar.h \
           pubkey/ecc_key/ecc_key.h \
           pubkey/ecdsa/ecdsa.h \
           pubkey/ecdsa/ecdsa_core.h \
           pubkey/ecdsa/ecdsa_op.h \
           pubkey/eckaeg/eckaeg.h \
           pubkey/eckaeg/eckaeg_core.h \
           pubkey/eckaeg/eckaeg_op.h \
           timer/gettimeofday/tm_unix.h
}

SOURCES += algo_factory/algo_factory.cpp \
        algo_factory/prov_weight.cpp \
        alloc/mem_pool/mem_pool.cpp \
        alloc/system_alloc/defalloc.cpp \
        asn1/alg_id.cpp \
        asn1/asn1_alt.cpp \
        asn1/asn1_att.cpp \
        asn1/asn1_dn.cpp \
        asn1/asn1_int.cpp \
        asn1/asn1_oid.cpp \
        asn1/asn1_str.cpp \
        asn1/asn1_tm.cpp \
        asn1/ber_dec.cpp \
        asn1/der_enc.cpp \
        benchmark/benchmark.cpp \
        block/aes/aes.cpp \
        block/aes/aes_tab.cpp \
        block/blowfish/blfs_tab.cpp \
        block/blowfish/blowfish.cpp \
        block/cast/cast128.cpp \
        block/cast/cast256.cpp \
        block/cast/cast_tab.cpp \
        block/des/des.cpp \
        block/des/des_tab.cpp \
        block/des/desx.cpp \
        block/gost_28147/gost_28147.cpp \
        block/idea/idea.cpp \
        block/kasumi/kasumi.cpp \
        block/lion/lion.cpp \
        block/lubyrack/lubyrack.cpp \
        block/mars/mars.cpp \
        block/mars/mars_tab.cpp \
        block/misty1/misty1.cpp \
        block/noekeon/noekeon.cpp \
        block/rc2/rc2.cpp \
        block/rc5/rc5.cpp \
        block/rc6/rc6.cpp \
        block/safer/safe_tab.cpp \
        block/safer/safer_sk.cpp \
        block/seed/seed.cpp \
        block/seed/seed_tab.cpp \
        block/serpent/serpent.cpp \
        block/skipjack/skipjack.cpp \
        block/square/sqr_tab.cpp \
        block/square/square.cpp \
        block/tea/tea.cpp \
        block/twofish/two_tab.cpp \
        block/twofish/twofish.cpp \
        block/xtea/xtea.cpp \
        cert/x509/certstor.cpp \
        cert/x509/crl_ent.cpp \
        cert/x509/pkcs10.cpp \
        cert/x509/x509_ca.cpp \
        cert/x509/x509_crl.cpp \
        cert/x509/x509_ext.cpp \
        cert/x509/x509_obj.cpp \
        cert/x509/x509cert.cpp \
        cert/x509/x509find.cpp \
        cert/x509/x509opt.cpp \
        cert/x509/x509self.cpp \
        cert/x509/x509stor.cpp \
        checksum/adler32/adler32.cpp \
        checksum/crc24/crc24.cpp \
        checksum/crc32/crc32.cpp \
        cms/cms_algo.cpp \
        cms/cms_comp.cpp \
        cms/cms_dalg.cpp \
        cms/cms_dec.cpp \
        cms/cms_ealg.cpp \
        cms/cms_enc.cpp \
        codec/base64/b64_char.cpp \
        codec/base64/base64.cpp \
        codec/hex/hex.cpp \
        codec/hex/hex_char.cpp \
        codec/openpgp/openpgp.cpp \
        codec/pem/pem.cpp \
        cryptobox/cryptobox.cpp \
        engine/def_engine/def_mode.cpp \
        engine/def_engine/def_pk_ops.cpp \
        engine/def_engine/def_powm.cpp \
        engine/def_engine/lookup_block.cpp \
        engine/def_engine/lookup_hash.cpp \
        engine/def_engine/lookup_mac.cpp \
        engine/def_engine/lookup_stream.cpp \
        filters/algo_filt.cpp \
        filters/basefilt.cpp \
        filters/buf_filt.cpp \
        filters/data_snk.cpp \
        filters/data_src.cpp \
        filters/filter.cpp \
        filters/out_buf.cpp \
        filters/pipe.cpp \
        filters/pipe_io.cpp \
        filters/pipe_rw.cpp \
        filters/secqueue.cpp \
        hash/fork256/fork256.cpp \
        hash/gost_3411/gost_3411.cpp \
        hash/has160/has160.cpp \
        hash/md2/md2.cpp \
        hash/md4/md4.cpp \
        hash/md5/md5.cpp \
        hash/mdx_hash/mdx_hash.cpp \
        hash/par_hash/par_hash.cpp \
        hash/rmd128/rmd128.cpp \
        hash/rmd160/rmd160.cpp \
        hash/sha1/sha160.cpp \
        hash/sha2/sha2_32.cpp \
        hash/sha2/sha2_64.cpp \
        hash/skein/skein_512.cpp \
        hash/tiger/tig_tab.cpp \
        hash/tiger/tiger.cpp \
        hash/whirlpool/whrl_tab.cpp \
        hash/whirlpool/whrlpool.cpp \
        kdf/kdf.cpp \
        kdf/kdf1/kdf1.cpp \
        kdf/kdf2/kdf2.cpp \
        kdf/mgf1/mgf1.cpp \
        kdf/ssl_prf/prf_ssl3.cpp \
        kdf/tls_prf/prf_tls.cpp \
        kdf/x942_prf/prf_x942.cpp \
        libstate/get_enc.cpp \
        libstate/init.cpp \
        libstate/libstate.cpp \
        libstate/look_pk.cpp \
        libstate/lookup.cpp \
        libstate/oid_lookup/oids.cpp \
        libstate/pk_engine.cpp \
        libstate/policy.cpp \
        libstate/scan_name.cpp \
        mac/cbc_mac/cbc_mac.cpp \
        mac/cmac/cmac.cpp \
        mac/hmac/hmac.cpp \
        mac/mac.cpp \
        mac/ssl3mac/ssl3_mac.cpp \
        mac/x919_mac/x919_mac.cpp \
        math/bigint/big_code.cpp \
        math/bigint/big_io.cpp \
        math/bigint/big_ops2.cpp \
        math/bigint/big_ops3.cpp \
        math/bigint/big_rand.cpp \
        math/bigint/bigint.cpp \
        math/bigint/divide.cpp \
        math/bigint/monty_generic/mp_monty.cpp \
        math/bigint/mp_asm.cpp \
        math/bigint/mp_comba.cpp \
        math/bigint/mp_karat.cpp \
        math/bigint/mp_misc.cpp \
        math/bigint/mp_shift.cpp \
        math/bigint/mulop_generic/mp_mulop.cpp \
        math/numbertheory/blinding.cpp \
        math/numbertheory/dsa_gen.cpp \
        math/numbertheory/jacobi.cpp \
        math/numbertheory/make_prm.cpp \
        math/numbertheory/mp_numth.cpp \
        math/numbertheory/numthry.cpp \
        math/numbertheory/pow_mod.cpp \
        math/numbertheory/powm_fw.cpp \
        math/numbertheory/powm_mnt.cpp \
        math/numbertheory/primes.cpp \
        math/numbertheory/reducer.cpp \
        math/numbertheory/ressol.cpp \
        modes/cbc/cbc.cpp \
        modes/cfb/cfb.cpp \
        modes/ctr/ctr.cpp \
        modes/cts/cts.cpp \
        modes/eax/eax.cpp \
        modes/eax/eax_dec.cpp \
        modes/ecb/ecb.cpp \
        modes/mode_pad/mode_pad.cpp \
        modes/modebase.cpp \
        modes/ofb/ofb.cpp \
        modes/xts/xts.cpp \
        mutex/noop_mutex/mux_noop.cpp \
        pbe/get_pbe.cpp \
        pbe/pbes1/pbes1.cpp \
        pbe/pbes2/pbes2.cpp \
        pk_pad/eme.cpp \
        pk_pad/eme1/eme1.cpp \
        pk_pad/eme_pkcs/eme_pkcs.cpp \
        pk_pad/emsa1/emsa1.cpp \
        pk_pad/emsa1_bsi/emsa1_bsi.cpp \
        pk_pad/emsa2/emsa2.cpp \
        pk_pad/emsa3/emsa3.cpp \
        pk_pad/emsa4/emsa4.cpp \
        pk_pad/emsa_raw/emsa_raw.cpp \
        pk_pad/hash_id/hash_id.cpp \
        pubkey/dh/dh.cpp \
        pubkey/dh/dh_core.cpp \
        pubkey/dl_algo/dl_algo.cpp \
        pubkey/dl_group/dl_group.cpp \
        pubkey/dlies/dlies.cpp \
        pubkey/dsa/dsa.cpp \
        pubkey/dsa/dsa_core.cpp \
        pubkey/dsa/dsa_op.cpp \
        pubkey/elgamal/elg_core.cpp \
        pubkey/elgamal/elg_op.cpp \
        pubkey/elgamal/elgamal.cpp \
        pubkey/if_algo/if_algo.cpp \
        pubkey/if_algo/if_core.cpp \
        pubkey/if_algo/if_op.cpp \
        pubkey/keypair/keypair.cpp \
        pubkey/nr/nr.cpp \
        pubkey/nr/nr_core.cpp \
        pubkey/nr/nr_op.cpp \
        pubkey/pk_algs.cpp \
        pubkey/pk_codecs/pkcs8.cpp \
        pubkey/pk_codecs/x509_key.cpp \
        pubkey/pk_filts.cpp \
        pubkey/pk_keys.cpp \
        pubkey/pubkey.cpp \
        pubkey/pubkey_enums.cpp \
        pubkey/rsa/rsa.cpp \
        pubkey/rw/rw.cpp \
        rng/auto_rng/auto_rng.cpp \
        rng/hmac_rng/hmac_rng.cpp \
        rng/randpool/randpool.cpp \
        rng/rng.cpp \
        rng/x931_rng/x931_rng.cpp \
        s2k/pbkdf1/pbkdf1.cpp \
        s2k/pbkdf2/pbkdf2.cpp \
        s2k/pgps2k/pgp_s2k.cpp \
        s2k/s2k.cpp \
        selftest/selftest.cpp \
        stream/arc4/arc4.cpp \
        stream/salsa20/salsa20.cpp \
        stream/stream_cipher.cpp \
        stream/turing/tur_tab.cpp \
        stream/turing/turing.cpp \
        stream/wid_wake/wid_wake.cpp \
        sym_algo/symkey.cpp \
        timer/timer.cpp \
        utils/charset.cpp \
        utils/datastor/datastor.cpp \
        utils/exceptn.cpp \
        utils/mlock.cpp \
        utils/parsing.cpp \
        utils/ui.cpp \
        utils/util.cpp \
        utils/version.cpp

win32 {
SOURCES += entropy/cryptoapi_rng/es_capi.cpp \
           entropy/win32_stats/es_win32.cpp \
           mutex/win32_crit_section/mux_win32.cpp \
           timer/win32_query_perf_ctr/tm_win32.cpp
}

unix {
    SOURCES += alloc/alloc_mmap/mmap_mem.cpp \
           cert/cvc/asn1_eac_str.cpp \
           cert/cvc/asn1_eac_tm.cpp \
           cert/cvc/cvc_ado.cpp \
           cert/cvc/cvc_ca.cpp \
           cert/cvc/cvc_cert.cpp \
           cert/cvc/cvc_req.cpp \
           cert/cvc/cvc_self.cpp \
           cert/cvc/ecdsa_sig.cpp \
           cert/cvc/signed_obj.cpp \
           entropy/dev_random/es_dev.cpp \
           entropy/egd/es_egd.cpp \
           entropy/proc_walk/es_ftw.cpp \
           entropy/unix_procs/es_unix.cpp \
           entropy/unix_procs/unix_cmd.cpp \
           entropy/unix_procs/unix_src.cpp \
           filters/fd_unix/fd_unix.cpp \
           math/gfpmath/curve_gfp.cpp \
           math/gfpmath/gfp_element.cpp \
           math/gfpmath/point_gfp.cpp \
           mutex/pthreads/mux_pthr.cpp \
           pubkey/ec_dompar/ec_dompar.cpp \
           pubkey/ecc_key/ecc_key.cpp \
           pubkey/ecdsa/ecdsa.cpp \
           pubkey/ecdsa/ecdsa_core.cpp \
           pubkey/ecdsa/ecdsa_op.cpp \
           pubkey/eckaeg/eckaeg.cpp \
           pubkey/eckaeg/eckaeg_core.cpp \
           pubkey/eckaeg/eckaeg_op.cpp \
           timer/gettimeofday/tm_unix.cpp
}

linux* {
    SOURCES += timer/posix_rt/tm_posix.cpp
}

os2 {
    SOURCES += \
           cert/cvc/asn1_eac_str.cpp \
           cert/cvc/asn1_eac_tm.cpp \
           cert/cvc/cvc_ado.cpp \
           cert/cvc/cvc_ca.cpp \
           cert/cvc/cvc_cert.cpp \
           cert/cvc/cvc_req.cpp \
           cert/cvc/cvc_self.cpp \
           cert/cvc/ecdsa_sig.cpp \
           cert/cvc/signed_obj.cpp \
           entropy/dev_random/es_dev.cpp \
           entropy/proc_walk/es_ftw.cpp \
           entropy/os2_stats/es_os2.cpp \
           filters/fd_unix/fd_unix.cpp \
           math/gfpmath/curve_gfp.cpp \
           math/gfpmath/gfp_element.cpp \
           math/gfpmath/point_gfp.cpp \
           mutex/qt_mutex/mux_qt.cpp \
           pubkey/ec_dompar/ec_dompar.cpp \
           pubkey/ecc_key/ecc_key.cpp \
           pubkey/ecdsa/ecdsa.cpp \
           pubkey/ecdsa/ecdsa_core.cpp \
           pubkey/ecdsa/ecdsa_op.cpp \
           pubkey/eckaeg/eckaeg.cpp \
           pubkey/eckaeg/eckaeg_core.cpp \
           pubkey/eckaeg/eckaeg_op.cpp \
           timer/gettimeofday/tm_unix.cpp
}

linux* {
    LIBS += -lrt
}

