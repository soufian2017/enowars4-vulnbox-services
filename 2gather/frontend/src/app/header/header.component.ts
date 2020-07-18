import { Component, OnInit } from '@angular/core';
import {BackendService} from '../backend.service';
import {User} from '../_models/user';
import {Select, Store} from "@ngxs/store";
import {LoginState} from "../login.state";
import {Observable} from "rxjs";
import {DeleteLogin} from "../login.actions";

@Component({
  selector: 'app-header',
  templateUrl: './header.component.html',
  styleUrls: ['./header.component.css']
})
export class HeaderComponent implements OnInit {
  @Select(LoginState.getLoginState) login$;

  user: User;
  constructor(private readonly backend: BackendService, private store: Store) { }

  // todo: add a store to handle the user and login logout state
  ngOnInit(): void {
    this.user = this.backend.userValue;
  }

  logout() {
    this.store.dispatch(new DeleteLogin());
  }
}
