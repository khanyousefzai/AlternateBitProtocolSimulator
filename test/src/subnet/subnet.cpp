#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>

#include <cadmium/modeling/coupling.hpp>
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/concept/coupled_model_assert.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_atomic.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/tuple_to_ostream.hpp>
#include <cadmium/logger/common_loggers.hpp>


#include "../../../lib/DESTimes/include/NDTime.hpp"
#include "../../../lib/vendor/iestream.hpp"

#include "../../../include/data_structures/message.hpp"
#include "../../../include/simulator_renaissance.hpp"

#include "../../../include/subnet_cadmium.hpp"

using namespace std;

using hclock=chrono::high_resolution_clock;
using TIME = NDTime;

/**Output file path of the subnet test*/
char output_file[] = "test/data/subnet_test_output.txt";
/**Output file path of the function output_file_evolution*/
char mod_output_file[] = "test/data/subnet_mod_output.csv";
/**Output file path of the function output_time_statistics*/
char time_statistics_file[] = "./data/time_stats_output.csv";


/***** SETING INPUT PORTS FOR COUPLEDs *****/
struct inp_in : public cadmium::in_port<message_t>{};

/***** SETING OUTPUT PORTS FOR COUPLEDs *****/
struct outp_out: public cadmium::out_port<message_t>{};

/********************************************/
/****** APPLICATION GENERATOR *******************/
/********************************************/
template<typename T>
class ApplicationGen : public iestream_input<message_t,T> {
public:
  ApplicationGen() = default;
  ApplicationGen(const char* file_path) : iestream_input<message_t,T>(file_path) {}
};


int main(){

  auto start = hclock::now(); //to measure simulation execution time

/*************** Loggers *******************/
  static std::ofstream out_data(output_file);
    struct oss_sink_provider{
        static std::ostream& sink(){          
            return out_data;
        }
    };

using info=cadmium::logger::logger<cadmium::logger::logger_info, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
using debug=cadmium::logger::logger<cadmium::logger::logger_debug, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
using state=cadmium::logger::logger<cadmium::logger::logger_state, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
using log_messages=cadmium::logger::logger<cadmium::logger::logger_messages, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
using routing=cadmium::logger::logger<cadmium::logger::logger_message_routing, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
using global_time=cadmium::logger::logger<cadmium::logger::logger_global_time, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
using local_time=cadmium::logger::logger<cadmium::logger::logger_local_time, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
using log_all=cadmium::logger::multilogger<info, debug, state, log_messages, routing, global_time, local_time>;

using logger_top=cadmium::logger::multilogger<log_messages, global_time>;


/*******************************************/



/********************************************/
/****** APPLICATION GENERATOR *******************/
/********************************************/
string input_data = "test/data/subnet_input_test.txt";
const char * i_input_data = input_data.c_str();

std::shared_ptr<cadmium::dynamic::modeling::model> generator = cadmium::dynamic::translate::make_dynamic_atomic_model<ApplicationGen, TIME, const char* >("generator" , std::move(i_input_data));


/********************************************/
/****** SUBNET *******************/
/********************************************/

std::shared_ptr<cadmium::dynamic::modeling::model> subnet1 = cadmium::dynamic::translate::make_dynamic_atomic_model<Subnet, TIME>("subnet1");


/************************/
/*******TOP MODEL********/
/************************/
cadmium::dynamic::modeling::Ports iports_TOP = {};
cadmium::dynamic::modeling::Ports oports_TOP = {typeid(outp_out)};
cadmium::dynamic::modeling::Models submodels_TOP = {generator, subnet1};
cadmium::dynamic::modeling::EICs eics_TOP = {};
cadmium::dynamic::modeling::EOCs eocs_TOP = {
  cadmium::dynamic::translate::make_EOC<subnet_defs::out,outp_out>("subnet1")
};
cadmium::dynamic::modeling::ICs ics_TOP = {
  cadmium::dynamic::translate::make_IC<iestream_input_defs<message_t>::out,subnet_defs::in>("generator","subnet1")
};
std::shared_ptr<cadmium::dynamic::modeling::coupled<TIME>> TOP = std::make_shared<cadmium::dynamic::modeling::coupled<TIME>>(
 "TOP", 
 submodels_TOP, 
 iports_TOP, 
 oports_TOP, 
 eics_TOP, 
 eocs_TOP, 
 ics_TOP 
  );

///****************////

    auto elapsed1 = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(hclock::now() - start).count();
    cout << "Model Created. Elapsed time: " << elapsed1 << "sec" << endl;
    
    cadmium::dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    elapsed1 = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(hclock::now() - start).count();
    cout << "Runner Created. Elapsed time: " << elapsed1 << "sec" << endl;

    cout << "Simulation starts" << endl;

    r.run_until(NDTime("04:00:00:000"));
    auto elapsed = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(hclock::now() - start).count();
    cout << "Simulation took:" << elapsed << "sec" << endl;

    /**
     * @brief          Function modifies the input file to more readable format 
     *
     * @param[in]      input_file   The input file is the ouput file of ABP or test sets
     * @param[in]      output_file  The output file is modified to more readable format
     */
    output_file_evolution(output_file, mod_output_file);

    /**
     * @brief      Time delay statistics
     * The function calculates the time delay statistics between the sender and receiver correspondence
     *
     * @param      mod_output_file   The input file is the ouput file of abp_mod_output function
     * @param      time_statistics_file  The output file is file containing statistics of time delay
     */
    output_time_statistics(mod_output_file, time_statistics_file);

    return 0;
}