import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import * as $ from 'jquery';

@Injectable({
  providedIn: 'root'
})
export class MbedClientService {

  _url = 'http://139.179.42.41';
  constructor(private _http: HttpClient) { }

  getDht() {
    return this._http.get(`${this._url}/dht`);
  }

  getFan() {
    return this._http.get(`${this._url}/fan`);
  }

  getRoof() {
    return this._http.get(`${this._url}/roof`);
  }

  getRequiredTemperature() {
    return this._http.get(`${this._url}/reqTemp`);
  }

  getRequiredHumidity() {
    return this._http.get(`${this._url}/reqHum`);
  }

  setTemperature(t: number) {
    console.log('in mbed client: ');
    return this._http.post(`${this._url}/t`, t + '').subscribe(x => { console.log('x: ', x); });
    // $.post(`${this._url}/t`, t + '', function () { console.log('post succ') }, 'text/plain');
    // $.post(`${this._url}/t`, t + '', function () { console.log('post succ') }, 'text/plain');
  }

  setHumidity(h: number) {
    $.post(`${this._url}/h`, h + '', function () { console.log('post succ') }, 'text/plain');
    $.post(`${this._url}/h`, h + '', function () { console.log('post succ') }, 'text/plain');
  }

}
