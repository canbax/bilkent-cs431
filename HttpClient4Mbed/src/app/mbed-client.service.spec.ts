import { TestBed } from '@angular/core/testing';

import { MbedClientService } from './mbed-client.service';

describe('MbedClientService', () => {
  beforeEach(() => TestBed.configureTestingModule({}));

  it('should be created', () => {
    const service: MbedClientService = TestBed.get(MbedClientService);
    expect(service).toBeTruthy();
  });
});
