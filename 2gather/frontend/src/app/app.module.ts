import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { SearchComponent } from './search/search.component';
import {MatCardModule} from '@angular/material/card';
import {MatFormFieldModule} from '@angular/material/form-field';
import { HeaderComponent } from './header/header.component';
import {MatToolbarModule} from '@angular/material/toolbar';
import {MatIconModule} from '@angular/material/icon';
import {MatButtonModule} from '@angular/material/button';
import { LoginComponent } from './login/login.component';
import {FlexModule} from '@angular/flex-layout';
import { RegisterComponent } from './register/register.component';
import {MatListModule} from '@angular/material/list';
import { LandingpageComponent } from './landingpage/landingpage.component';
import {HTTP_INTERCEPTORS, HttpClientModule} from '@angular/common/http';
import {FormsModule} from "@angular/forms";
import {MatInputModule} from "@angular/material/input";
import {JwtInterceptor} from "./_helpers/jwt.interceptor";
import { LoginDisplayComponent } from './login/login-display/login-display.component';
import {MatSelectModule} from "@angular/material/select";
import { DisplayRegisterComponent } from './register/display-register/display-register.component';
import {MatMenuModule} from "@angular/material/menu";
import { WallOfLuovComponent } from './wall-of-luov/wall-of-luov.component';
import { FooterComponent } from './footer/footer.component';
import {MatExpansionModule} from "@angular/material/expansion";
import {MatRippleModule} from "@angular/material/core";
import { AboutUsComponent } from './about-us/about-us.component';
import {NgxsModule} from "@ngxs/store";
import {LoginState} from "./login.state";
import {environment} from "../environments/environment";

@NgModule({
  declarations: [
    AppComponent,
    SearchComponent,
    HeaderComponent,
    LoginComponent,
    RegisterComponent,
    LandingpageComponent,
    LoginDisplayComponent,
    DisplayRegisterComponent,
    WallOfLuovComponent,
    FooterComponent,
    AboutUsComponent
  ],
    imports: [
        BrowserModule,
        AppRoutingModule,
        BrowserAnimationsModule,
        MatCardModule,
        MatFormFieldModule,
        MatToolbarModule,
        MatIconModule,
        MatButtonModule,
        FlexModule,
        MatListModule,
        HttpClientModule,
        FormsModule,
        MatInputModule,
        MatSelectModule,
        MatMenuModule,
        MatExpansionModule,
        MatRippleModule,
        NgxsModule.forRoot([LoginState], {
          developmentMode: !environment.production
        })
    ],
  providers: [{ provide: HTTP_INTERCEPTORS, useClass: JwtInterceptor, multi: true }],
  bootstrap: [AppComponent]
})
export class AppModule { }
