/** Minimal macro based example! */ 


//  gSystem->Load("${ARA_UTIL_INSTALL_DIR}/lib/libAraEvent.so");  (I think the CVMFS environment loads this already, otherwise you may need this) 

TH1 * exercise1(const char * event_file ="/data/exp/ARA/2016/unblinded/L1/ARA02/0221/run006910/event006910.root", int rfchan = 0, int max_entry = 1000) 
{

  auto h = new TH1D("snrs","SNRSs",20,0,10); 

  TFile f(event_file); 
  auto eventTree = (TTree*) f.Get("eventTree"); 

  RawAtriStationEvent * raw = 0; 
  eventTree->SetBranchAddress("event",&raw); 

  for (int i = 0; i < eventTree->GetEntries(); i++) 
  {

    eventTree->GetEntry(i); 
    if ((i % 100) == 0) 
    {
      printf("%d\n",i); 
    }

    if (max_entry >= 0 && i > max_entry) break; 
    if (raw->isCalpulserEvent()) continue; 


    UsefulAtriStationEvent useful(raw, AraCalType::kLatestCalib); 
    

    auto g = useful.getGraphFromRFChan(rfchan); 

    double rms = g->GetRMS(2); //second axis
    double max = *std::max_element(g->GetY(), g->GetY()+g->GetN());
    double min = *std::min_element(g->GetY(), g->GetY()+g->GetN());


    double Vpp = max-min; 

    h->Fill(Vpp/(2*rms)); 
    delete g; 
  } 

  h->Draw(); 

  return h; 
}


