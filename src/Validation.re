module type Foldable = {
  type t('a);
  let concat: (t('a), t('a)) => t('a);
};

module Make_validation = (F: Foldable) => {
  type t('a, 'b) =
    | Failure(F.t('a))
    | Success('b);

  let map = (transform, t) =>
    switch t {
    | Failure(x) => Failure(x)
    | Success(x) => Success(transform(x))
    };

  /*
    forall a, b, c:
      (Validation (b) => c).(Validation a b) => Validation a c
  */
  let apply = (aValidation, t) =>
    switch t {
    | Failure(x) => switch aValidation {
      | Failure(y) => Failure(F.concat(x, y))
      | Success(_) => Failure(x)
      }
    | Success(x) => switch aValidation {
      | Failure(y) => Failure(y)
      | Success(_) => map(x, aValidation)
      }
    };

  /* forall a, b: (Validation a b).() => b :: throws TypeError */
  let unsafeGet = fun
    | Failure(_) => failwith("Can't extract the value of a Failure")
    | Success(x) => x;

  /* forall a, b: (Validation a b).(b) => b */
  let getOrElse = (defaultValue, t) =>
    switch t {
    | Failure(_) => defaultValue
    | Success(x) => x
    };

  /*
    forall a, b, c:
      (Validation a b).((a) => Validation c b) => Validation c b
  */
  let orElse = (handler, t) =>
    switch t {
    | Failure(x) => handler(x)
    | Success(x) => Success(x)
    };

  /*
    forall a, b:
      (Validation a b).(Validation a b) => Validation a b
  */
  let concat = (aValidation, t) =>
    switch t {
    | Failure(x) => switch aValidation {
      | Failure(y) => Failure(F.concat(x, y))
      | Success(_) => Failure(x)
      }
    | Success(_) => aValidation
    };

  /*
    forall a, b, c:
      (Validation a b).((a) => c, (b) => c) => c
  */
  let fold = (failure, success, t) =>
    switch t {
    | Failure(x) => failure(x)
    | Success(x) => success(x)
    };

  /*
    forall a, b:
      (Validation a b).() => Validation b a
  */
  let swap = fun
    | Failure(x) => Success(x)
    | Success(x) => Failure(x);

  /*
    forall a, b, c, d:
      (Validation a b).((a) => c, (b) => d) => Validation c d
  */
  let bimap = (failure, success, t) =>
    switch t {
    | Failure(x) => Failure(failure(x))
    | Success(x) => Success(success(x))
    };

  /*
    forall a, b, c:
      (Validation a b).((a) => c) => Validation c b
  */
  let mapFailure = (transform, t) =>
    switch t {
    | Failure(x) => Failure(transform(x))
    | Success(x) => Success(x)
    };

  /*
    forall a, b:
      (b) => Validation a b
  */
  let make = x => Success(x);

  /*
    # Not sure how to implement this.
    forall a, b:
      (Validation a, b).() => a or b
  let merge = fun
    | Failure(x) => x
    | Success(x) => x
  */

  /*
    forall a, b:
      (Validation a b).() => option(b)
  */
  let toOption = fun
    | Failure(_) => None
    | Success(x) => Some(x);
};
