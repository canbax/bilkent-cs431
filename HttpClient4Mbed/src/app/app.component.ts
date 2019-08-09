import { Component } from '@angular/core';
import { MbedClientService } from './mbed-client.service';


@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  serverResponse = '';
  temperature = 25;
  humidity = 60;
  url = '';

  constructor(public srv: MbedClientService) {
    this.url = this.srv.url;
  }

  // read sensors
  getDht() {
    this.serverResponse = '...';
    this.srv.getDht().subscribe(x => {
      this.serverResponse = JSON.stringify(x);
    });
  }

  getFan() {
    this.serverResponse = '...';
    this.srv.getFan().subscribe(x => {
      this.serverResponse = x.toString();
    });
  }

  getRoof() {
    this.serverResponse = '...';
    this.srv.getRoof().subscribe(x => {
      this.serverResponse = x.toString();
    });
  }

  getRequiredTemp() {
    this.serverResponse = '...';
    this.srv.getRequiredTemperature().subscribe(x => {
      this.serverResponse = x.toString();
    });
  }

  // set ideal temperature
  setTemperature(t: number) {
    this.serverResponse = '...';
    this.srv.setTemperature(t).subscribe(x => {
      this.serverResponse = x.toString();
    });
  }

  // set ideal humidity
  setHumidity(h: number) {
    this.serverResponse = '...';
    this.srv.setHumidity(h).subscribe(x => {
      this.serverResponse = x.toString();
    });
  }

  setUrl(url: string){
    this.srv.url = url;
    this.url = url;
  }

}
