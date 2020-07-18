import {LoginStateModel} from './login.state';

export class AddLogin{
  static readonly type = '[Login] Set login true';
  constructor() {}
}

export class DeleteLogin {
  static readonly type = '[Login] Set login to false';
  constructor() {}
}
