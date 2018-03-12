[![Build Status](https://www.travis-ci.org/scull7/bs-validation.svg?branch=master)](https://www.travis-ci.org/scull7/bs-validation)
[![Coverage Status](https://coveralls.io/repos/github/scull7/bs-validation/badge.svg?branch=master)](https://coveralls.io/github/scull7/bs-validation?branch=master)

# bs-validation
A BuckleScript implementation of the [Folktale][folktale]
[validation][folktale-validation] applicative

***NOTE:*** These are ***not*** bindings, this a ReasonML implementation of the
`Validation` applicative.

## Why?
I wanted a way to do validations for my server side project.

## Status
Not all of the [Folktale/Validation][folktale-validation] functions will be
implemented. Here is a list of the currently implemented functions:

* [x] map
* [x] apply
* [x] unsafeGet
* [x] getOrElse
* [x] orElse
* [x] concat
* [x] fold
* [x] swap
* [x] bimap
* [x] mapFailure
* [x] toOption
* [ ] ~~merge~~ *see note in the code.*

All implemented functions are found in `src/Validation.re`, They are all
documented with their [Folktale][folktale] style doc strings.

## How do I install?
1. Add the [bs-validation][npm-bs-validation] package to your project.
  ```sh
  yarn add bs-validation
  ```
2. Add `bs-validation` to your `bsconfig.json`
  ```json
  {
    "dependencies": [ "bs-validation" ]
  }
  ```
3. Enjoy!

## Usage

The library is exposed as a functor which accepts modules that implement the
following type interface:
```reason
module type Foldable = {
  type t('a);
  let concat: (t('a), t('a)) => t('a);
};
```

All of the examples use an array based implementation of the `Foldable` type:
```reason
module FoldableArray = {
  type t('a) = array('a);
  let concat = (x, y) => Belt_Array.concat(x, y);
};
```

You import the module into your project by calling the `Validation` functor
with your version of the `Foldable` type.
```reason
module V = Validation.Make_validation(FoldableArray);
```

Then you can use it to validate all of your things!
```reason
let lengthError = "Password must have more than 6 characters.";
let strengthError = "Password must contain a special character.";

let isPasswordLongEnough = (password) =>
  String.length(password) > 6
    ? V.Success(password)
    : V.Failure([|lengthError|]);

let isPasswordStrongEnough = (password) => {
  let regex = [%bs.re "/[\\W]/"];
  Js.Re.test(password, regex)
    ? V.Success(password)
    : V.Failure([|strengthError|])
};

let isPasswordValid = (password) => {
  V.Success()
  |> V.concat(isPasswordLongEnough(password))
  |> V.concat(isPasswordStrongEnough(password))
  |> V.map((_) =>password)
};


describe("Folketale password validation example", () => {
  test("should return the password", () => {
    let password = "rosesarered$andstuff";
    switch (isPasswordValid(password)) {
    | Failure(f) => { Js.log(f); fail("unexpected_failure") }
    | Success(p) => Expect.expect(p) |> Expect.toBe(password)
    }
  });

  test("should return a single item failure", () => {
    let password = "rosesarered";
    switch (isPasswordValid(password)) {
    | Failure(f) => Expect.expect(f) |> Expect.toBeSupersetOf([|strengthError|])
    | Success(_) => fail("unexpected_success")
    }
  });

  test("should return 2 items in the failure list", () => {
    let password = "foo";
    switch (isPasswordValid(password)) {
    | Failure(f) => {
        Expect.expect(f)
        |> Expect.toBeSupersetOf([|lengthError, strengthError|])
      }
    | Success(_) => fail("unexpected_success")
    }
  });
});
```

[folktale]: http://folktale.origamitower.com/
[folktale-validation]: http://folktale.origamitower.com/api/v2.1.0/en/folktale.validation.html
[npm-bs-validation]: https://www.npmjs.com/package/bs-validation
