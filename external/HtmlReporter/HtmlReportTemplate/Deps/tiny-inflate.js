/*
MIT License

Copyright (c) 2015-present Devon Govett
Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
var zi = source => {
    var createArray = (i) => new Uint16Array(i);
    var createTree = () => {
        return {
            t: createArray(16),   /* table of code length counts */
            r: createArray(288)  /* code -> symbol translation table */
        }
    };

    var sourceIndex = 0;
    var getSource = () => sourceIndex < source.length ? source[sourceIndex++].codePointAt(0) : 0;
    var tag = 0;
    var bc = 0;
    var dest = [];
    var writeDest = i => {
        dest.push(String.fromCharCode(i))
    };

    var ltree = createTree();  /* dynamic length/symbol tree */
    var dtree = createTree();  /* dynamic distance tree */

    /* --------------------------------------------------- *
     * -- uninitialized global data (static structures) -- *
     * --------------------------------------------------- */

    var sltree = createTree();
    var sdtree = createTree();

    /* extra bits and base tables for length codes */
    var length_bits = createArray(30);
    var length_base = createArray(30);

    /* extra bits and base tables for distance codes */
    var dist_bits = createArray(30);
    var dist_base = createArray(30);

    /* special ordering of code length codes */
    var clcidx = [
        16, 17, 18, 0, 8, 7, 9, 6,
        10, 5, 11, 4, 12, 3, 13, 2,
        14, 1, 15
    ];

    /* used by tinf_decode_trees, avoids allocations every call */
    var code_tree = createTree();
    var lengths = createArray(288 + 32);

    /* ----------------------- *
     * -- utility functions -- *
     * ----------------------- */
    var i, sum;
    /* build extra bits and base tables */
    var tinf_build_bits_base = (bits, base, delta, first) => {

        /* build bits table */
        bits.fill(0);
        for (i = 0; i < 30 - delta; ++i) bits[i + delta] = i / delta | 0;

        /* build base table */
        for (sum = first, i = 0; i < 30; ++i) {
            base[i] = sum;
            sum += 1 << bits[i];
        }
    }

    /* build the fixed huffman trees */
    var tinf_build_fixed_trees = (lt, dt) => {
        /* build fixed length tree */
        lt.t.fill(0);

        lt.t[7] = 24;
        lt.t[8] = 152;
        lt.t[9] = 112;

        for (i = 0; i < 24; ++i) lt.r[i] = 256 + i;
        for (i = 0; i < 144; ++i) lt.r[24 + i] = i;
        for (i = 0; i < 8; ++i) lt.r[24 + 144 + i] = 280 + i;
        for (i = 0; i < 112; ++i) lt.r[24 + 144 + 8 + i] = 144 + i;

        /* build fixed distance tree */
        dt.t.fill(0);

        dt.t[5] = 32;

        for (i = 0; i < 32; ++i) dt.r[i] = i;
    }

    /* given an array of code lengths, build a tree */
    var offs = createArray(16);

    var tinf_build_tree = (t, lengths, off, num) => {
        /* clear code length count table */
        t.t.fill(0);

        /* scan symbol lengths, and sum code length counts */
        for (i = 0; i < num; ++i) t.t[lengths[off + i]]++;

        t.t[0] = 0;

        /* compute offset table for distribution sort */
        for (sum = 0, i = 0; i < 16; ++i) {
            offs[i] = sum;
            sum += t.t[i];
        }

        /* create code->symbol translation table (symbols sorted by code) */
        for (i = 0; i < num; ++i) {
            if (lengths[off + i]) t.r[offs[lengths[off + i]]++] = i;
        }
    }

    /* ---------------------- *
     * -- decode functions -- *
     * ---------------------- */

    /* get one bit from source stream */
    var tinf_getbit = () => {
        /* check if tag is empty */
        if (!bc--) {
            /* load next tag */
            tag = getSource();
            bc = 7;
        }

        /* shift bit out of tag */
        var bit = tag & 1;
        tag >>>= 1;

        return bit;
    }

    /* read a num bit value from a stream and add base */
    var tinf_read_bits = (num, base) => {
        if (!num)
            return base;

        while (bc < 24) {
            tag |= getSource() << bc;
            bc += 8;
        }

        var val = tag & (0xffff >>> (16 - num));
        tag >>>= num;
        bc -= num;
        return val + base;
    }

    /* given a data stream and a tree, decode a symbol */
    var tinf_decode_symbol = t => {
        while (bc < 24) {
            tag |= getSource() << bc;
            bc += 8;
        }

        var sum = 0, cur = 0, len = 0;
        var ltag = tag;

        /* get more bits while code value is above sum */
        do {
            cur = 2 * cur + (ltag & 1);
            ltag >>>= 1;
            ++len;

            sum += t.t[len];
            cur -= t.t[len];
        } while (cur >= 0);

        tag = ltag;
        bc -= len;

        return t.r[sum + cur];
    }

    /* given a data stream, decode dynamic trees from it */
    var tinf_decode_trees = (lt, dt) => {
        var hlit, hdist, hclen;
        var i, num, length, sym, prev;

        /* get 5 bits HLIT (257-286) */
        hlit = tinf_read_bits(5, 257);

        /* get 5 bits HDIST (1-32) */
        hdist = tinf_read_bits(5, 1);

        /* get 4 bits HCLEN (4-19) */
        hclen = tinf_read_bits(4, 4);

        lengths.fill(0);
        /* read code lengths for code length alphabet */
        for (i = 0; i < hclen; ++i) {
            /* get 3 bits code length (0-7) */
            lengths[clcidx[i]] = tinf_read_bits(3, 0);
        }

        /* build code length tree */
        tinf_build_tree(code_tree, lengths, 0, 19);

        /* decode code lengths for the dynamic trees */
        for (num = 0; num < hlit + hdist;) {
            sym = tinf_decode_symbol(code_tree);
            if (sym == 16) {
                /* copy previous code length 3-6 times (read 2 bits) */
                prev = lengths[num - 1];
                for (length = tinf_read_bits(2, 3); length; --length) {
                    lengths[num++] = prev;
                }
            } else if (sym == 17)
                /* repeat code length 0 for 3-10 times (read 3 bits) */
                for (length = tinf_read_bits(3, 3); length; --length) {
                    lengths[num++] = 0;
                }
            else if (sym == 18)
                /* repeat code length 0 for 11-138 times (read 7 bits) */
                for (length = tinf_read_bits(7, 11); length; --length) {
                    lengths[num++] = 0;
                }
            else
                lengths[num++] = sym;/* values 0-15 represent the actual code lengths */
        }

        /* build dynamic trees */
        tinf_build_tree(lt, lengths, 0, hlit);
        tinf_build_tree(dt, lengths, hlit, hdist);
    }

    /* ----------------------------- *
     * -- block inflate functions -- *
     * ----------------------------- */

    /* given a stream and two trees, inflate a block of data */
    var tinf_inflate_block_data = (lt, dt) => {
        while (1) {
            var sym = tinf_decode_symbol(lt);

            /* check for end of block */
            if (sym === 256) {
                return;
            }

            if (sym < 256) {
                writeDest(sym);
            } else {
                var length, dist, offs;
                var i;

                sym -= 257;

                /* possibly get more bits from length code */
                length = tinf_read_bits(length_bits[sym], length_base[sym]);

                dist = tinf_decode_symbol(dt);

                /* possibly get more bits from distance code */
                offs = dest.length - tinf_read_bits(dist_bits[dist], dist_base[dist]);

                /* copy match */
                for (i = offs; i < offs + length; ++i) {
                    dest.push(dest[i]);
                }
            }
        }
    }

    /* inflate an uncompressed block of data */
    var tinf_inflate_uncompressed_block = () => {
        var length, invlength;

        /* unread from bitbuffer */
        while (bc > 8) {
            sourceIndex--;
            bc -= 8;
        }
        /* get length */
        length = getSource() + 256 * getSource();

        /* get one's complement of length */
        invlength = getSource() + 256 * getSource();

        /* check length */
        if (length !== (~invlength & 0x0000ffff))
            throw -3;

        /* copy block */
        for (var i = length; i; --i)
            writeDest(getSource());

        /* make sure we start next block on a byte boundary */
        bc = 0;
    }
    var bfinal, btype;

    /* -------------------- *
     * -- initialization -- *
     * -------------------- */

    /* build fixed huffman trees */
    tinf_build_fixed_trees(sltree, sdtree);

    /* build extra bits and base tables */
    tinf_build_bits_base(length_bits, length_base, 4, 3);
    tinf_build_bits_base(dist_bits, dist_base, 2, 1);

    /* fix a special case */
    length_bits[28] = 0;
    length_base[28] = 258;

    do {
        /* read final block flag */
        bfinal = tinf_getbit();

        /* read block type (2 bits) */
        btype = tinf_read_bits(2, 0);

        /* decompress block */
        if (btype == 0)
            tinf_inflate_uncompressed_block();
        else if (btype == 1)
            tinf_inflate_block_data(sltree, sdtree);
        else {
            tinf_decode_trees(ltree, dtree);
            tinf_inflate_block_data(ltree, dtree);
        }
    } while (!bfinal);
    return dest.join("");
}