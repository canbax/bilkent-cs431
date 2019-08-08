import { Component } from '@angular/core';
import { MbedClientService } from './mbed-client.service';


@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  serverResponse = '...';
  temperature = 25;
  humidity = 60;

  constructor(private _srv: MbedClientService) {
  }

  // read sensors
  getDht() {
    this.serverResponse = '...';
    this._srv.getDht().subscribe(x => {
      this.serverResponse = JSON.stringify(x);
    });
  }

  getFan() {
    this.serverResponse = '...';
    this._srv.getFan().subscribe(x => {
      this.serverResponse = x.toString();
    });
  }

  getRoof() {
    this.serverResponse = '...';
    this._srv.getRoof().subscribe(x => {
      this.serverResponse = x.toString();
    });
  }

  getRequiredTemp() {
    this.serverResponse = '...';
    this._srv.getRequiredTemperature().subscribe(x => {
      this.serverResponse = x.toString();
    });
  }

  // set ideal temperature
  setTemperature(t: number) {
    console.log('setTemperature: ');
    this._srv.setTemperature(t);
  }

  // set ideal humidity
  setHumidity(h: number) {
    this._srv.setHumidity(h);
  }

  setUrl(url: string){
    this._srv._url = url;
  }

}
