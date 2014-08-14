;yottos_snowball_cache = typeof yottos_snowball_cache !== 'undefined' ? yottos_snowball_cache : {};
;function win2unicode(str) {
   var charmap   = unescape(
      "\u0402\u0403\u201A\u0453\u201E\u2026\u2020\u2021\u20AC\u2030\u0409\u2039\u040A\u040C\u040B\u040F"+
      "\u0452\u2018\u2019\u201C\u201D\u2022\u2013\u2014\u0098\u2122\u0459\u203A\u045A\u045C\u045B\u045F"+
      "\u00A0\u040E\u045E\u0408\u00A4\u0490\u00A6\u00A7\u0401\u00A9\u0404\u00AB\u00AC\u00AD\u00AE\u0407"+
      "\u00B0\u00B1\u0406\u0456\u0491\u00B5\u00B6\u00B7\u0451\u2116\u0454\u00BB\u0458\u0405\u0455\u0457");
   var code2char = function(code) {
               if(code >= 0xC0 && code <= 0xFF) return String.fromCharCode(code - 0xC0 + 0x0410)
               if(code >= 0x80 && code <= 0xBF) return charmap.charAt(code - 0x80)
               return String.fromCharCode(code)
            }
   var res = ""
   for(var i = 0; i < str.length; i++) res = res + code2char(str.charCodeAt(i))
   return res;
}
;function validate(string)
{
    string = win2unicode(string);
    var utftext = "";
    for (var n = 0; n < string.length; n++) {
        var c = string.charCodeAt(n);
            if((c > 47) && (c < 58))
            {
                utftext += String.fromCharCode(c);
            }
            else if ((c > 64) && (c < 91))
            {
                utftext += String.fromCharCode(c);
            }
            else if ((c > 96) && (c < 123))
            {
                utftext += String.fromCharCode(c);
            }
            else if ((c > 1039) && (c < 1104))
            {
                utftext += String.fromCharCode(c);
            }
            else
            {
                utftext += String.fromCharCode(32);
            }
    }
    return utftext;
}
;function getContext(){
    ;function yottos_Snowball(lng) {
            function Among(s, substring_i, result, method) {
                this.s_size = s.length;
                this.s = this.toCharArray(s);
                this.substring_i = substring_i;
                this.result = result;
                this.method = method;
            }
            Among.prototype.toCharArray = function(s) {
                var sLength = s.length, charArr = new Array(sLength);
                for (var i = 0; i < sLength; i++)
                    charArr[i] = s.charCodeAt(i);
                return charArr;
            }
            function SnowballProgram() {
                var current;
                return {
                    b : 0,
                    k : 0,
                    l : 0,
                    c : 0,
                    lb : 0,
                    s_c : function(word) {
                        current = word;
                        this.c = 0;
                        this.l = word.length;
                        this.lb = 0;
                        this.b = this.c;
                        this.k = this.l;
                    },
                    g_c : function() {
                        var result = current;
                        current = null;
                        return result;
                    },
                    i_g : function(s, min, max) {
                        if (this.c < this.l) {
                            var ch = current.charCodeAt(this.c);
                            if (ch <= max && ch >= min) {
                                ch -= min;
                                if (s[ch >> 3] & (0X1 << (ch & 0X7))) {
                                    this.c++;
                                    return true;
                                }
                            }
                        }
                        return false;
                    },
                    i_g_b : function(s, min, max) {
                        if (this.c > this.lb) {
                            var ch = current.charCodeAt(this.c - 1);
                            if (ch <= max && ch >= min) {
                                ch -= min;
                                if (s[ch >> 3] & (0X1 << (ch & 0X7))) {
                                    this.c--;
                                    return true;
                                }
                            }
                        }
                        return false;
                    },
                    o_g : function(s, min, max) {
                        if (this.c < this.l) {
                            var ch = current.charCodeAt(this.c);
                            if (ch > max || ch < min) {
                                this.c++;
                                return true;
                            }
                            ch -= min;
                            if (!(s[ch >> 3] & (0X1 << (ch & 0X7)))) {
                                this.c++;
                                return true;
                            }
                        }
                        return false;
                    },
                    o_g_b : function(s, min, max) {
                        if (this.c > this.lb) {
                            var ch = current.charCodeAt(this.c - 1);
                            if (ch > max || ch < min) {
                                this.c--;
                                return true;
                            }
                            ch -= min;
                            if (!(s[ch >> 3] & (0X1 << (ch & 0X7)))) {
                                this.c--;
                                return true;
                            }
                        }
                        return false;
                    },
                    e_s : function(s_size, s) {
                        if (this.l - this.c < s_size)
                            return false;
                        for (var i = 0; i < s_size; i++)
                            if (current.charCodeAt(this.c + i) != s.charCodeAt(i))
                                return false;
                        this.c += s_size;
                        return true;
                    },
                    e_s_b : function(s_size, s) {
                        if (this.c - this.lb < s_size)
                            return false;
                        for (var i = 0; i < s_size; i++)
                            if (current.charCodeAt(this.c - s_size + i) != s
                                    .charCodeAt(i))
                                return false;
                        this.c -= s_size;
                        return true;
                    },
                    f_a : function(v, v_size) {
                        var i = 0, j = v_size, c = this.c, l = this.l, common_i = 0, common_j = 0, first_key_inspected = false;
                        while (true) {
                            var k = i + ((j - i) >> 1), diff = 0, common = common_i < common_j
                                    ? common_i
                                    : common_j, w = v[k];
                            for (var i2 = common; i2 < w.s_size; i2++) {
                                if (c + common == l) {
                                    diff = -1;
                                    break;
                                }
                                diff = current.charCodeAt(c + common) - w.s[i2];
                                if (diff)
                                    break;
                                common++;
                            }
                            if (diff < 0) {
                                j = k;
                                common_j = common;
                            } else {
                                i = k;
                                common_i = common;
                            }
                            if (j - i <= 1) {
                                if (i > 0 || j == i || first_key_inspected)
                                    break;
                                first_key_inspected = true;
                            }
                        }
                        while (true) {
                            var w = v[i];
                            if (common_i >= w.s_size) {
                                this.c = c + w.s_size;
                                if (!w.method)
                                    return w.result;
                                var res = w.method();
                                this.c = c + w.s_size;
                                if (res)
                                    return w.result;
                            }
                            i = w.substring_i;
                            if (i < 0)
                                return 0;
                        }
                    },
                    f_a_b : function(v, v_size) {
                        var i = 0, j = v_size, c = this.c, lb = this.lb, common_i = 0, common_j = 0, first_key_inspected = false;
                        while (true) {
                            var k = i + ((j - i) >> 1), diff = 0, common = common_i < common_j
                                    ? common_i
                                    : common_j, w = v[k];
                            for (var i2 = w.s_size - 1 - common; i2 >= 0; i2--) {
                                if (c - common == lb) {
                                    diff = -1;
                                    break;
                                }
                                diff = current.charCodeAt(c - 1 - common) - w.s[i2];
                                if (diff)
                                    break;
                                common++;
                            }
                            if (diff < 0) {
                                j = k;
                                common_j = common;
                            } else {
                                i = k;
                                common_i = common;
                            }
                            if (j - i <= 1) {
                                if (i > 0 || j == i || first_key_inspected)
                                    break;
                                first_key_inspected = true;
                            }
                        }
                        while (true) {
                            var w = v[i];
                            if (common_i >= w.s_size) {
                                this.c = c - w.s_size;
                                if (!w.method)
                                    return w.result;
                                var res = w.method();
                                this.c = c - w.s_size;
                                if (res)
                                    return w.result;
                            }
                            i = w.substring_i;
                            if (i < 0)
                                return 0;
                        }
                    },
                    r_s : function(c_bra, c_ket, s) {
                        var adjustment = s.length - (c_ket - c_bra), left = current
                                .substring(0, c_bra), right = current.substring(c_ket);
                        current = left + s + right;
                        this.l += adjustment;
                        if (this.c >= c_ket)
                            this.c += adjustment;
                        else if (this.c > c_bra)
                            this.c = c_bra;
                        return adjustment;
                    },
                    s_ch : function() {
                        if (this.b < 0 || this.b > this.k || this.k > this.l
                                || this.l > current.length)
                            throw ("faulty slice operation");
                    },
                    s_f : function(s) {
                        this.s_ch();
                        this.r_s(this.b, this.k, s);
                    },
                    s_d : function() {
                        this.s_f("");
                    },
                    i_ : function(c_bra, c_ket, s) {
                        var adjustment = this.r_s(c_bra, c_ket, s);
                        if (c_bra <= this.b)
                            this.b += adjustment;
                        if (c_bra <= this.k)
                            this.k += adjustment;
                    },
                    s_t : function() {
                        this.s_ch();
                        return current.substring(this.b, this.k);
                    },
                    e_v_b : function(s) {
                        return this.e_s_b(s.length, s);
                    }
                };
            }
            var stemFactory = {
                RussianStemmer : function() {
                    var a_0 = [new Among("\u0432", -1, 1),
                            new Among("\u0438\u0432", 0, 2),
                            new Among("\u044B\u0432", 0, 2),
                            new Among("\u0432\u0448\u0438", -1, 1),
                            new Among("\u0438\u0432\u0448\u0438", 3, 2),
                            new Among("\u044B\u0432\u0448\u0438", 3, 2),
                            new Among("\u0432\u0448\u0438\u0441\u044C", -1, 1),
                            new Among("\u0438\u0432\u0448\u0438\u0441\u044C", 6, 2),
                            new Among("\u044B\u0432\u0448\u0438\u0441\u044C", 6, 2)], a_1 = [
                            new Among("\u0435\u0435", -1, 1),
                            new Among("\u0438\u0435", -1, 1),
                            new Among("\u043E\u0435", -1, 1),
                            new Among("\u044B\u0435", -1, 1),
                            new Among("\u0438\u043C\u0438", -1, 1),
                            new Among("\u044B\u043C\u0438", -1, 1),
                            new Among("\u0435\u0439", -1, 1),
                            new Among("\u0438\u0439", -1, 1),
                            new Among("\u043E\u0439", -1, 1),
                            new Among("\u044B\u0439", -1, 1),
                            new Among("\u0435\u043C", -1, 1),
                            new Among("\u0438\u043C", -1, 1),
                            new Among("\u043E\u043C", -1, 1),
                            new Among("\u044B\u043C", -1, 1),
                            new Among("\u0435\u0433\u043E", -1, 1),
                            new Among("\u043E\u0433\u043E", -1, 1),
                            new Among("\u0435\u043C\u0443", -1, 1),
                            new Among("\u043E\u043C\u0443", -1, 1),
                            new Among("\u0438\u0445", -1, 1),
                            new Among("\u044B\u0445", -1, 1),
                            new Among("\u0435\u044E", -1, 1),
                            new Among("\u043E\u044E", -1, 1),
                            new Among("\u0443\u044E", -1, 1),
                            new Among("\u044E\u044E", -1, 1),
                            new Among("\u0430\u044F", -1, 1),
                            new Among("\u044F\u044F", -1, 1)], a_2 = [
                            new Among("\u0435\u043C", -1, 1),
                            new Among("\u043D\u043D", -1, 1),
                            new Among("\u0432\u0448", -1, 1),
                            new Among("\u0438\u0432\u0448", 2, 2),
                            new Among("\u044B\u0432\u0448", 2, 2),
                            new Among("\u0449", -1, 1),
                            new Among("\u044E\u0449", 5, 1),
                            new Among("\u0443\u044E\u0449", 6, 2)], a_3 = [
                            new Among("\u0441\u044C", -1, 1),
                            new Among("\u0441\u044F", -1, 1)], a_4 = [
                            new Among("\u043B\u0430", -1, 1),
                            new Among("\u0438\u043B\u0430", 0, 2),
                            new Among("\u044B\u043B\u0430", 0, 2),
                            new Among("\u043D\u0430", -1, 1),
                            new Among("\u0435\u043D\u0430", 3, 2),
                            new Among("\u0435\u0442\u0435", -1, 1),
                            new Among("\u0438\u0442\u0435", -1, 2),
                            new Among("\u0439\u0442\u0435", -1, 1),
                            new Among("\u0435\u0439\u0442\u0435", 7, 2),
                            new Among("\u0443\u0439\u0442\u0435", 7, 2),
                            new Among("\u043B\u0438", -1, 1),
                            new Among("\u0438\u043B\u0438", 10, 2),
                            new Among("\u044B\u043B\u0438", 10, 2),
                            new Among("\u0439", -1, 1),
                            new Among("\u0435\u0439", 13, 2),
                            new Among("\u0443\u0439", 13, 2),
                            new Among("\u043B", -1, 1),
                            new Among("\u0438\u043B", 16, 2),
                            new Among("\u044B\u043B", 16, 2),
                            new Among("\u0435\u043C", -1, 1),
                            new Among("\u0438\u043C", -1, 2),
                            new Among("\u044B\u043C", -1, 2),
                            new Among("\u043D", -1, 1),
                            new Among("\u0435\u043D", 22, 2),
                            new Among("\u043B\u043E", -1, 1),
                            new Among("\u0438\u043B\u043E", 24, 2),
                            new Among("\u044B\u043B\u043E", 24, 2),
                            new Among("\u043D\u043E", -1, 1),
                            new Among("\u0435\u043D\u043E", 27, 2),
                            new Among("\u043D\u043D\u043E", 27, 1),
                            new Among("\u0435\u0442", -1, 1),
                            new Among("\u0443\u0435\u0442", 30, 2),
                            new Among("\u0438\u0442", -1, 2),
                            new Among("\u044B\u0442", -1, 2),
                            new Among("\u044E\u0442", -1, 1),
                            new Among("\u0443\u044E\u0442", 34, 2),
                            new Among("\u044F\u0442", -1, 2),
                            new Among("\u043D\u044B", -1, 1),
                            new Among("\u0435\u043D\u044B", 37, 2),
                            new Among("\u0442\u044C", -1, 1),
                            new Among("\u0438\u0442\u044C", 39, 2),
                            new Among("\u044B\u0442\u044C", 39, 2),
                            new Among("\u0435\u0448\u044C", -1, 1),
                            new Among("\u0438\u0448\u044C", -1, 2),
                            new Among("\u044E", -1, 2),
                            new Among("\u0443\u044E", 44, 2)], a_5 = [
                            new Among("\u0430", -1, 1),
                            new Among("\u0435\u0432", -1, 1),
                            new Among("\u043E\u0432", -1, 1),
                            new Among("\u0435", -1, 1),
                            new Among("\u0438\u0435", 3, 1),
                            new Among("\u044C\u0435", 3, 1),
                            new Among("\u0438", -1, 1),
                            new Among("\u0435\u0438", 6, 1),
                            new Among("\u0438\u0438", 6, 1),
                            new Among("\u0430\u043C\u0438", 6, 1),
                            new Among("\u044F\u043C\u0438", 6, 1),
                            new Among("\u0438\u044F\u043C\u0438", 10, 1),
                            new Among("\u0439", -1, 1),
                            new Among("\u0435\u0439", 12, 1),
                            new Among("\u0438\u0435\u0439", 13, 1),
                            new Among("\u0438\u0439", 12, 1),
                            new Among("\u043E\u0439", 12, 1),
                            new Among("\u0430\u043C", -1, 1),
                            new Among("\u0435\u043C", -1, 1),
                            new Among("\u0438\u0435\u043C", 18, 1),
                            new Among("\u043E\u043C", -1, 1),
                            new Among("\u044F\u043C", -1, 1),
                            new Among("\u0438\u044F\u043C", 21, 1),
                            new Among("\u043E", -1, 1), new Among("\u0443", -1, 1),
                            new Among("\u0430\u0445", -1, 1),
                            new Among("\u044F\u0445", -1, 1),
                            new Among("\u0438\u044F\u0445", 26, 1),
                            new Among("\u044B", -1, 1), new Among("\u044C", -1, 1),
                            new Among("\u044E", -1, 1),
                            new Among("\u0438\u044E", 30, 1),
                            new Among("\u044C\u044E", 30, 1),
                            new Among("\u044F", -1, 1),
                            new Among("\u0438\u044F", 33, 1),
                            new Among("\u044C\u044F", 33, 1)], a_6 = [
                            new Among("\u043E\u0441\u0442", -1, 1),
                            new Among("\u043E\u0441\u0442\u044C", -1, 1)], a_7 = [
                            new Among("\u0435\u0439\u0448\u0435", -1, 1),
                            new Among("\u043D", -1, 2),
                            new Among("\u0435\u0439\u0448", -1, 1),
                            new Among("\u044C", -1, 3)], g_v = [33, 65, 8, 232], I_p2, I_pV, sbp = new SnowballProgram();
                    this.setCurrent = function(word) {
                        sbp.s_c(word);
                    };
                    this.getCurrent = function() {
                        return sbp.g_c();
                    };
                    function habr3() {
                        while (!sbp.i_g(g_v, 1072, 1103)) {
                            if (sbp.c >= sbp.l)
                                return false;
                            sbp.c++;
                        }
                        return true;
                    }
                    function habr4() {
                        while (!sbp.o_g(g_v, 1072, 1103)) {
                            if (sbp.c >= sbp.l)
                                return false;
                            sbp.c++;
                        }
                        return true;
                    }
                    function r_mark_regions() {
                        I_pV = sbp.l;
                        I_p2 = I_pV;
                        if (habr3()) {
                            I_pV = sbp.c;
                            if (habr4())
                                if (habr3())
                                    if (habr4())
                                        I_p2 = sbp.c;
                        }
                    }
                    function r_R2() {
                        return I_p2 <= sbp.c;
                    }
                    function habr2(a, n) {
                        var a_v, v_1;
                        sbp.k = sbp.c;
                        a_v = sbp.f_a_b(a, n);
                        if (a_v) {
                            sbp.b = sbp.c;
                            switch (a_v) {
                                case 1 :
                                    v_1 = sbp.l - sbp.c;
                                    if (!sbp.e_s_b(1, "\u0430")) {
                                        sbp.c = sbp.l - v_1;
                                        if (!sbp.e_s_b(1, "\u044F"))
                                            return false;
                                    }
                                case 2 :
                                    sbp.s_d();
                                    break;
                            }
                            return true;
                        }
                        return false;
                    }
                    function r_perfective_gerund() {
                        return habr2(a_0, 9);
                    }
                    function habr1(a, n) {
                        var a_v;
                        sbp.k = sbp.c;
                        a_v = sbp.f_a_b(a, n);
                        if (a_v) {
                            sbp.b = sbp.c;
                            if (a_v == 1)
                                sbp.s_d();
                            return true;
                        }
                        return false;
                    }
                    function r_adjective() {
                        return habr1(a_1, 26);
                    }
                    function r_adjectival() {
                        var a_v;
                        if (r_adjective()) {
                            habr2(a_2, 8);
                            return true;
                        }
                        return false;
                    }
                    function r_reflexive() {
                        return habr1(a_3, 2);
                    }
                    function r_verb() {
                        return habr2(a_4, 46);
                    }
                    function r_noun() {
                        habr1(a_5, 36);
                    }
                    function r_derivational() {
                        var a_v;
                        sbp.k = sbp.c;
                        a_v = sbp.f_a_b(a_6, 2);
                        if (a_v) {
                            sbp.b = sbp.c;
                            if (r_R2() && a_v == 1)
                                sbp.s_d();
                        }
                    }
                    function r_tidy_up() {
                        var a_v;
                        sbp.k = sbp.c;
                        a_v = sbp.f_a_b(a_7, 4);
                        if (a_v) {
                            sbp.b = sbp.c;
                            switch (a_v) {
                                case 1 :
                                    sbp.s_d();
                                    sbp.k = sbp.c;
                                    if (!sbp.e_s_b(1, "\u043D"))
                                        break;
                                    sbp.b = sbp.c;
                                case 2 :
                                    if (!sbp.e_s_b(1, "\u043D"))
                                        break;
                                case 3 :
                                    sbp.s_d();
                                    break;
                            }
                        }
                    }
                    this.stem = function() {
                        r_mark_regions();
                        sbp.c = sbp.l;
                        if (sbp.c < I_pV)
                            return false;
                        sbp.lb = I_pV;
                        if (!r_perfective_gerund()) {
                            sbp.c = sbp.l;
                            if (!r_reflexive())
                                sbp.c = sbp.l;
                            if (!r_adjectival()) {
                                sbp.c = sbp.l;
                                if (!r_verb()) {
                                    sbp.c = sbp.l;
                                    r_noun();
                                }
                            }
                        }
                        sbp.c = sbp.l;
                        sbp.k = sbp.c;
                        if (sbp.e_s_b(1, "\u0438")) {
                            sbp.b = sbp.c;
                            sbp.s_d();
                        } else
                            sbp.c = sbp.l;
                        r_derivational();
                        sbp.c = sbp.l;
                        r_tidy_up();
                        return true;
                    }
                }}
            var stemName = lng.substring(0, 1).toUpperCase()
                    + lng.substring(1).toLowerCase() + "Stemmer";
            return new stemFactory[stemName]();
    }
    ;var yottos_Stem = function(lng) {
        var yottos_testStemmer = new yottos_Snowball(lng);
        return function(word) {
          yottos_testStemmer.setCurrent(word);
          yottos_testStemmer.stem();
          return yottos_testStemmer.getCurrent();
        }
    };
    ;function yottos_stremer(word){
        if (yottos_snowball_cache[word]){
            w = yottos_snowball_cache[word];
        }
        else{
            w = new yottos_Stem('russian')(word);
            yottos_snowball_cache[word] = w;
        }
        return w;
    };
    function getText(){
        var result = '';
        result += (document.title.replace(/[^a-zA-Zа-яА-Я]/g,' ').replace(/\s+$/g,'').replace(/^\s+/g,'').replace(/[\n\t\r\f\s]{2,}/g,' '));
        var metas = document.getElementsByTagName('meta');
        if (metas) {
            for (var x=0,y=metas.length; x<y; x++) {
                if (metas[x].name.toLowerCase() == "description") {
                    result += ' ';
                    result += (metas[x].content.replace(/[^a-zA-Zа-яА-Я]/g,' ').replace(/\s+$/g,'').replace(/^\s+/g,'').replace(/[\n\t\r\f\s]{2,}/g,' ')) + ' ';
                }
                if (metas[x].name.toLowerCase() == "keywords") {
                    result += ' ';
                    result += (metas[x].content.replace(/[^a-zA-Zа-яА-Я]/g,' ').replace(/\s+$/g,'').replace(/^\s+/g,'').replace(/[\n\t\r\f\s]{2,}/g,' ')) + ' ';
                }
            }
        }
       return result;
    }
    var yottos_splitted = getText().toLowerCase().split(' ');
    var yottos_collector = {};
    var yottos_counter = {};
    var yottos_ignore = ['описание','отзыв','рейтинг','характеристик','параметр','продает','тест','бол','больш','будет','будут','как','пок','коментар','будт','был','быт','вдруг','вед','впроч','всегд','всег','всех',
    'говор','главн','даж','друг','дальш','добав','есл','ест','жизн','зач','зде','иногд','кажет','кажд','как','когд','конечн','котор','куд',
    'лучш','либ','межд','мен','долж','смысл','след','чита','люб','постара','помим','числ','соб','ждат','част','использ','велик','член','некотор',
    'мног','может','можн','наконец','нег','нельз','нибуд','никогд','нич','один','опя','опубликова','перед','посл','пот','почт','разв','сво',
    'себ','сегодн','сейчас','сказа','совс','так','теб','тепер','тогд','тог','тож','тольк','хорош','хот','чег','человек','пут','вполн','возможн',
    'через','чтоб','чут','эт','позж','все','поэт','точн','этот','над','итог','недел','сведен','тем','город','гроз','зон','принят','балл','перв',
    'вход','лент','оста','мир','образ','идет','выйт','нол','сил','наш','мнен','одн','ищ','выш','взял','откр','нов','удив','всем','важн','ожида',
    'сам','ход','пущ','тег','выж','комментар','ваш','цен','коснут','слаб','ситуац','назов','уход','дол','основн','ряд','заверш','дополнен','влия',
    'описа','меньш','двум','слаб','стал','новост','отраз','вопрос','выбор','сдела','смерт','последн','поворот','высок','опозор','текст',
    'назов','основн','причин','групп','похож','with','that','this','about','above','after','again','against','because','been','before','being',
    'below','between','both','cannot','could','does','down','than','that','important','partner','border','link','text','radius','none','document',
    'height','color','title','normal','font','down','display','width','block','margin','yandex','item','type','left','padding','auto','inner',
    'function','decorati','google','position','http','align','webkit','inherit','direct','hover','referrer','write','size','math','spacing',
    'line','sizing','float','bottom','vert','charset','vertical','background','underline','visited','inline','unescape','value','style','visible',
    'address','else','true','tail','iframe','adriver','white','space','collapse','content','list','window','absolute','script','random','transparent',
    'repeat','scroll','container','piclayout','email','site','form','location','place','table','show','indent','visibility','clickable','last',
    'agewarn','opts','toggler','errormsg','getcode','href','relative','overflow','clear','cursor','outline','index','full','baseline','hide',
    'focus','catch','async','https','escape','round','target','blank','frameborder','scrolling','click','hidden','empty','cells','letter','static',
    'layout','transform','word','right','weight','warn','active','used','context','undefined','counter','page','mail','body','domain','region',
    'pointer','nowrap','family','first','data','online','push','metrika','callbacks','image','classname','protocol','init','icon','wrap','root',
    'solid','facebook','options','defaults','offset','false','return','like','opera','frames','typeof','decoration'];
        yottos_ignore = (function(){
            var yottos_o = {};
            var yottos_iCount = yottos_ignore.length;
            for (var i=0;i<yottos_iCount;i++){
                yottos_o[yottos_ignore[i]] = true;
            }
            return yottos_o;
        }());
        for (i = 0; i < yottos_splitted.length; i++) {
           var yottos_key = yottos_splitted[i].replace(/^\s*/, "").replace(/\s*$/, "");
           if (yottos_key.length > 3){
                var yottos_streem_key = yottos_stremer(yottos_key);
                if (!yottos_ignore[yottos_streem_key]) {
                    yottos_collector[yottos_streem_key] = yottos_key;
                    yottos_counter[yottos_streem_key] = yottos_counter[yottos_streem_key] || 0;
                    yottos_counter[yottos_streem_key]++;
                }
            }
        }
        var yottos_arr = [];
        for (yottos_sWord in yottos_counter) {
            if (yottos_counter[yottos_sWord] > 1){
                yottos_arr.push({
                text: yottos_collector[yottos_sWord],
                streem:yottos_sWord,
                frequency: yottos_counter[yottos_sWord]
                });
            }
        }
        var yottos_sort_arr = yottos_arr.sort(function(a,b){return (a.frequency > b.frequency) ? -1 : ((a.frequency < b.frequency) ? 1 : 0);});
        var yottos_out = [];
        for (var i=0; i<yottos_sort_arr.length; i++) {
            yottos_out.push(yottos_sort_arr[i].text);
        }

        ;var yottos_out_length = 0;
        ;var yottos_output = [];
        for (var i=0; i<yottos_out.length; i++)
        {
            yottos_out_length += yottos_out[i].length;
            if (navigator.appName == 'Microsoft Internet Explorer'){
                if (yottos_out_length < 200) {
                    yottos_output.push(yottos_out[i]);
                }
            }
            else{
                if (yottos_out_length < 1000) {
                    yottos_output.push(yottos_out[i]);
                }
            }
        }
    return (yottos_output.join(' '));
}
;function yottos_getSearch(){
    ;var result = "";
    ;var search_engines=[
        {"name":"Mail", "pattern":"go.mail.ru", "param":"q"},
        {"name":"Google", "pattern":"google.", "param":"q"},
        {"name":"Google", "pattern":"google.", "param":"as_q"},
        {"name":"Live Search", "pattern":"search.live.com", "param":"q"},
        {"name":"Bing", "pattern":"bing", "param":"q"},
        {"name":"Rambler", "pattern":"rambler.ru", "param":"query"},
        {"name":"Rambler", "pattern":"rambler.ru", "param":"words"},
        {"name":"Yahoo!", "pattern":"search.yahoo.com", "param":"p"},
        {"name":"Nigma", "pattern":"nigma.ru", "param":"s"},
        {"name":"Nigma", "pattern":"nigma.ru", "param":"q"},
        {"name":"MSN", "pattern":"search.msn.com", "param":"q"},
        {"name":"Ask", "pattern":"ask.com", "param":"q"},
        {"name":"QIP", "pattern":"search.qip.ru", "param":"query"},
        {"name":"RapidAll", "pattern":"rapidall.com", "param":"keyword"},
        {"name":"Яндекс.Картинки", "pattern":"images.yandex", "param":"text"},
        {"name":"Яндекс.Mobile", "pattern":"m.yandex", "param":"query"},
        {"name":"Яндекс", "pattern":"hghltd.yandex", "param":"text"},
        {"name":"Яндекс", "pattern":"yandex", "param":"text"},
        {"name":"УкрНет", "pattern":"ukr.net", "param":"q"},
        {"name":"УкрНет", "pattern":"ukr.net", "param":"q"},
        {"name":"meta", "pattern":"meta.ua", "param":"q"},
        {"name":"findes", "pattern":"findes.com.ua", "param":"q"},
        {"name":"webalta", "pattern":"webalta", "param":"q"},
        {"name":"bigmir", "pattern":"bigmir.net", "param":"z"},
        {"name":"i.ua", "pattern":"i.ua", "param":"q"},
        {"name":"online.ua", "pattern":"online.ua", "param":"q"},
        {"name":"liveinternet.ru", "pattern":"liveinternet.ru", "param":"q"},
        {"name":"all.by", "pattern":"all.by", "param":"query"}
    ];
    var parser = document.createElement('a');
    parser.href = document.referrer;
    for (var i=0; i<search_engines.length; i++)
    {
        if (-1 < parser.hostname.indexOf(search_engines[i]['pattern']))
        {
            var param = parser.search.replace(new RegExp("\\?",'g'),"").split('&');
            for (var y=0; y<param.length; y++)
            {
                if (param[y].split('=')[0] == search_engines[i]['param'])
                {
                    result = decodeURIComponent(param[y].split('=')[1]);
                }
            }
        }
    }
    return result;
}
;var y_url = window.location;
;var y_ref = document.createElement('a');
;y_ref.href = document.referrer;
;yottos_remarketing_time = typeof yottos_remarketing_time == 'number' ? yottos_remarketing_time : 365;
;yottos_ac = typeof yottos_ac == "string" ? yottos_ac : '';
if (typeof yottos_remarketing_redirect == "object")
{

    if (yottos_remarketing_redirect[y_url.toString()] != undefined)
    {
        ;y_url = document.createElement('a');
        ;y_url = yottos_remarketing_redirect[y_url.toString()]
    }
    else
    {
        if (yottos_remarketing_redirect[y_ref.toString()] != undefined)
        {
            ;y_url = document.createElement('a');
            ;y_url = yottos_remarketing_redirect[y_ref.toString()]
        }
    }
}
;function getOfferId()
{
	if(window.location.host == 'www.topshoptv.com.ua')
	{
		return product.identifier;
	}
	else if(window.location.host == 'ek.ua')
	{
		return google_tag_params.ecomm_prodid;
	}
	else if(window.location.host == 'rosetka.com.ua')
	{
		return dataLayer[3].productID;
	}
return;
}
;var rand = Math.floor(Math.random() * 1000000);
;var iframe_id = 'yottos_remarketing' + rand;
;function yottos_iframe_remarketing_query()
{
    return "?location=" + encodeURIComponent(y_url) + '&time=' + yottos_remarketing_time + '&ac=' + yottos_ac + '&rand=' + rand  + "&search=" + encodeURIComponent(validate(yottos_getSearch())) +  "&context=" + encodeURIComponent(validate(getContext())) +  "&offer_id=" + encodeURIComponent(validate(getOfferId()));
}
;document.write("<iframe id='" + iframe_id + "' src='http://rg.yottos.com/track.fcgi" + yottos_iframe_remarketing_query() + "' width='0' height='0'  frameborder='0' scrolling='no'></iframe>");
