// prefix.js

// (function (global, factory) {
//     var Module;
//     if(!Module) {
//         Module = (typeof Module !== "undefined" ? Module : null ) || {};
//     }

//     if (typeof module === "object" && typeof module.exports === "object") {
//         var v = factory(Module);
//         module.exports = Module;
//         // if (v !== undefined) module.exports = v;
//     } else {
//         var v = factory(Module);
//         global.oniguruma = Module;
//     }
// })(this, function (Module) {

// prefix.js
define(function() {
    return function(Module) {