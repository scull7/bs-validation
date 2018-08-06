// Generated by BUCKLESCRIPT VERSION 4.0.3, PLEASE EDIT WITH CARE
'use strict';

var Jest = require("@glennsl/bs-jest/src/jest.js");
var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Validation = require("../src/Validation.bs.js");

var concat = Belt_Array.concat;

var FoldableArray = /* module */[/* concat */concat];

var V = Validation.Make_validation(FoldableArray);

function make(name, compiler) {
  return /* record */[
          /* name */name,
          /* compiler */compiler
        ];
}

function toArray(t) {
  return /* array */[
          t[/* name */0],
          t[/* compiler */1]
        ];
}

var Language = /* module */[
  /* make */make,
  /* toArray */toArray
];

describe("Validation.apply", (function () {
        return Jest.test("Should properly handle the Folktale language example", (function () {
                      return Jest.Expect[/* toBeSupersetOf */9](/* array */[
                                  "Rust",
                                  "rustc"
                                ], Jest.Expect[/* expect */0](toArray(Curry._1(V[/* unsafeGet */2], Curry._2(V[/* apply */1], /* Success */Block.__(1, ["rustc"]), Curry._2(V[/* apply */1], /* Success */Block.__(1, ["Rust"]), /* Success */Block.__(1, [make])))))));
                    }));
      }));

exports.FoldableArray = FoldableArray;
exports.V = V;
exports.Language = Language;
/* V Not a pure module */
