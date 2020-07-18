import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { SearchComponent } from './search/search.component';
import {LoginComponent} from './login/login.component';
import {RegisterComponent} from './register/register.component';
import {LandingpageComponent} from './landingpage/landingpage.component';
import {LoginDisplayComponent} from './login/login-display/login-display.component';
import {AuthGuard} from './_helpers/auth.guard';
import {DisplayRegisterComponent} from './register/display-register/display-register.component';
import {AboutUsComponent} from './about-us/about-us.component';
import {WallOfLuovComponent} from './wall-of-luov/wall-of-luov.component';

const routes: Routes = [
  { path: '', component: LandingpageComponent },
  { path: 'search', component: SearchComponent},
  { path: 'login', component: LoginComponent },
  { path: 'register', component: RegisterComponent},
  { path: 'profile', component: LoginDisplayComponent, canActivate: [AuthGuard]},
  { path: 'register-display', component: DisplayRegisterComponent },
  { path: 'about-us', component: AboutUsComponent },
  { path: 'wall-of-luov', component: WallOfLuovComponent }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
