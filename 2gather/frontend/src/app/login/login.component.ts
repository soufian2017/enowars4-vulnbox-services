import { Component, OnInit } from '@angular/core';
import {BackendService} from '../backend.service';
import {Router} from '@angular/router';
import {Store} from "@ngxs/store";
import {AddLogin} from "../login.actions";

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit {
  // variables for to way binding to hold the input values
  // todo: switch to form with validation
  public pass: string;
  public user: string;

  // dependency injection of the backend service for communication
  // and the angular specific router
  constructor(private backend: BackendService, private router: Router, private store: Store) { }

  ngOnInit(): void {
  }

  // call of the login function on click of the login button in the html file
  onLogin() {
    // call the backend service, pass username and password
    // on response route to login/display
    this.backend.postLogin(this.user, this.pass).subscribe(data => {
      this.store.dispatch(new AddLogin());
      this.router.navigate(['/profile']);
    });
  }
}
