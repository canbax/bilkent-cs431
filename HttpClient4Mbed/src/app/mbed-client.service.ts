import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})
export class MbedClientService {

  url = 'http://139.179.42.41';
  constructor(private _http: HttpClient) { }

  getDht() {
    return this._http.get(`${this.url}/dht`);
  }

  getFan() {
    return this._http.get(`${this.url}/fan`);
  }

  getRoof() {
    return this._http.get(`${this.url}/roof`);
  }

  getRequiredTemperature() {
    return this._http.get(`${this.url}/reqTemp`);
  }

  getRequiredHumidity() {
    return this._http.get(`${this.url}/reqHum`);
  }

  setTemperature(t: number) {
    return this._http.post(`${this.url}/t`, t + '');
  }

  setHumidity(h: number) {
    return this._http.post(`${this.url}/h`, h + '');
  }

}
