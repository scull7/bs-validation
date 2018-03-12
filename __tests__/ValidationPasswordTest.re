open Jest;
/*
  Folketale/validation
  http://folktale.origamitower.com/api/v2.1.0/en/folktale.validation.html
*/
module FoldableArray = {
  type t('a) = array('a);
  let concat = (x, y) => Belt_Array.concat(x, y);
};

module V = Validation.Make_validation(FoldableArray);

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
