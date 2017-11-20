define('wasm-onig', ['oniguruma'], function(oniguruma) {
    oniguruma.OnigScanner2 = (function () {
        function OnigScanner(regexps) {
            this.vectorStr = new oniguruma.VectorString();
            for (var i = 0; i < regexps.length; i++) {
                this.vectorStr.push_back(regexps[i]);
            }
            this.scanner = new oniguruma.OnigScanner(this.vectorStr);
        }
        OnigScanner.prototype._findNextMatchSync = function (lin, pos) {
            if (pos == null) {
                pos = 0
            }
            var subject = lin;
            var maxTestStringSize = (subject.length * 2 + 1) * 2;
            var testString = oniguruma._malloc(maxTestStringSize);
            oniguruma.stringToUTF8(subject, testString, maxTestStringSize);

            // scanner.test(testString, subject.length);
            let match = this.scanner._findNextMatchSync(testString, subject.length, pos);
            if (match) match.scanner = this

            return match;
        };

        OnigScanner.prototype.convertToString = function (value) {
            if (value === undefined) return 'undefined'
            if (value === null) return 'null'
            return value.toString()
        }

        OnigScanner.prototype.convertToNumber = function (value) {
            value = parseInt(value)
            if (!isFinite(value)) {
                value = 0
            }
            value = Math.max(value, 0)
            return value
        }

        return OnigScanner;
    }());

    return oniguruma;
});