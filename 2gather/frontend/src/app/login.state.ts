import {State, Action, StateContext, Selector } from '@ngxs/store';
import {AddLogin, DeleteLogin} from './login.actions';
import {Injectable} from '@angular/core';

export class LoginStateModel {
  login: boolean;
}

@State<LoginStateModel>({
  name: 'login',
  defaults: {
    login: false
  }
})

@Injectable()
export class LoginState {

  @Selector()
  static getLoginState(state: LoginStateModel) {
    return state.login;
  }

  @Action(AddLogin)
  addLogin({patchState}: StateContext<LoginStateModel>) {
    patchState({
      login: true
    });
  }

  @Action(DeleteLogin)
  add({patchState}: StateContext<LoginStateModel>) {
    patchState({
      login: false
    });
  }
}
