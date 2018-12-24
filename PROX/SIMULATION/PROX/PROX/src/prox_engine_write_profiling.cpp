#include <prox_engine.h>
#include <prox_engine_data.h>

#include <util_profiling.h>
#include <util_write_matlab_profiling_data.h>
#include <util_log.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <string>
#include <vector>
#include <fstream>   // needed for std::ofstream

namespace prox
{

  bool Engine::write_matlab_profiling_data(std::string const & filename)
  {
    util::Log        logging;

    std::string const newline = util::Log::newline();

    std::ofstream matlab;

    matlab.open(filename.c_str(),std::ios::out);

    if(! matlab.is_open())
    {
      logging << "Engine::write_matlab_profiling_data(): error could not open file = " << filename.c_str() << util::Log::newline();

      return false;
    }

    matlab << "%close all;" << std::endl;
    matlab << "%clear all;" << std::endl;
    matlab << "%clc;"       << std::endl;
    matlab << std::endl;
    matlab << std::endl;
    matlab << util::write_matlab_profiling_data()  << std::endl;
    matlab << std::endl;
    matlab << std::endl;

    matlab << std::endl;
    matlab << std::endl;
    matlab <<  "%red   = [0.7,0.1,0.1];"   << std::endl;
    matlab <<  "%green = [0.1,0.7,0.1];" << std::endl;
    matlab <<  "%blue  = [0.1,0.1,0.7];"  << std::endl;
    matlab <<  "%f_type = 'Times';"      << std::endl;
    matlab <<  "%f_size = 20;"           << std::endl;
    matlab << std::endl;
    matlab << std::endl;
    matlab << "%frames = [1:length(Ekin)];" << std::endl;
    matlab << "%Emech = Epot + Ekin;" << std::endl;
    matlab << std::endl;
    matlab << std::endl;
    matlab << "%figure(100);" << std::endl;
    matlab << "%e1 = plot(frames, Ekin,'r-'); " << std::endl;
    matlab << "%hold on; " << std::endl;
    matlab << "%e2 = plot(frames, Epot, 'g-'); " << std::endl;
    matlab << "%e3 = plot(frames, Emech, 'b-'); " << std::endl;
    matlab << "%hold off; " << std::endl;
    matlab << "%title('Kinetic and potential energy of total system', 'FontSize', f_size, 'FontName', f_type); "  << std::endl;
    matlab << "%legend([e1,e2,e3], {'Kinetic', 'Potential','Mechanical'}, 'FontSize', f_size, 'FontName', f_type); "  << std::endl;
    matlab << "%ylabel('Energy (Joules)', 'FontSize', f_size, 'FontName', f_type)" << std::endl;
    matlab << "%xlabel('Frame', 'FontSize', f_size, 'FontName', f_type)" << std::endl;
    matlab << "%print(gcf,'-depsc2','energy');" << std::endl;
    matlab << "%print(gcf,'-dpng','energy');" << std::endl;
    matlab << std::endl;
    matlab << std::endl;
    matlab << "%figure(200);" << std::endl;
    matlab << "%plot(abs(average_penetration)*100);" << std::endl;
    matlab << "%title('Penetrations', 'FontSize', f_size, 'FontName', f_type); " << std::endl;
    matlab << "%xlabel('Frame', 'FontSize', f_size, 'FontName', f_type); " << std::endl;
    matlab << "%ylabel('Penetration depth (cm)', 'FontSize', f_size, 'FontName', f_type)" << std::endl;
    matlab << "%print(gcf,'-depsc2','penetration');" << std::endl;
    matlab << "%print(gcf,'-dpng','penetration');" << std::endl;
    matlab << std::endl;
    matlab << std::endl;
    matlab << "%figure(300);" << std::endl;
    matlab << "%plot(contacts);" << std::endl;
    matlab << "%title('Contacts', 'FontSize', f_size, 'FontName', f_type); " << std::endl;
    matlab << "%xlabel('Frame', 'FontSize', f_size, 'FontName', f_type); " << std::endl;
    matlab << "%ylabel('Number of contacts', 'FontSize', f_size, 'FontName', f_type)" << std::endl;
    matlab << "%print(gcf,'-depsc2','contacts');" << std::endl;
    matlab << "%print(gcf,'-dpng','contacts');" << std::endl;
    matlab << std::endl;
    matlab << std::endl;
    matlab << "%figure(400);" << std::endl;
    matlab << "%plot(contacts(contacts>0) ,solver, '.'); "<< std::endl;
    matlab << "%title('Time used by solver vs number of contacts', 'FontSize', f_size, 'FontName', f_type); "<< std::endl;
    matlab << "%xlabel('Number of contacts', 'FontSize', f_size, 'FontName', f_type); "<< std::endl;
    matlab << "%ylabel('Solver (ms)', 'FontSize', f_size, 'FontName', f_type)" << std::endl;
    matlab << "%print(gcf,'-depsc2','solver_time_vs_contacts');" << std::endl;
    matlab << "%print(gcf,'-dpng','solver_time_vs_contacts');" << std::endl;
    matlab << std::endl;
    matlab << std::endl;
    matlab << "%figure(500);" << std::endl;
    matlab << "%plot(frames(contacts>0), solver); " << std::endl;
    matlab << "%title('Time used by solver per frame', 'FontSize', f_size, 'FontName', f_type)" << std::endl;
    matlab << "%xlabel('Frame', 'FontSize', f_size, 'FontName', f_type); " << std::endl;
    matlab << "%ylabel('Solver (ms)', 'FontSize', f_size, 'FontName', f_type)" << std::endl;
    matlab << "%print(gcf,'-depsc2','solver_time_per_frame');" << std::endl;
    matlab << "%print(gcf,'-dpng','solver_time_per_frame');" << std::endl;
    matlab << std::endl;
    matlab << std::endl;
    matlab << "%figure(600);" << std::endl;
    matlab << "%semilogy(cell2mat(convergence(1)));" << std::endl;
    matlab << "%hold on" << std::endl;
    matlab << "%for frame = 2:numel(convergence); " << std::endl;
    matlab << "%  semilogy(cell2mat(convergence(frame)));" << std::endl;
    matlab << "%end" << std::endl;
    matlab << "%title('Convergence', 'FontSize', f_size, 'FontName',f_type );" << std::endl;
    matlab << "%xlabel('Solver iteration', 'FontSize', f_size, 'FontName', f_type); " << std::endl;
    matlab << "%ylabel('Natural merit function', 'FontSize', f_size, 'FontName', f_type);" << std::endl;
    matlab << "%print(gcf,'-depsc2','convergence');" << std::endl;
    matlab << "%print(gcf,'-dpng','convergence');" << std::endl;
    matlab << std::endl;
    matlab << std::endl;
    matlab << "%figure(700);" << std::endl;
    matlab << "%plot(cell2mat(rfactor(1)));" << std::endl;
    matlab << "%hold on" << std::endl;
    matlab << "%for frame = 2:numel(rfactor);" << std::endl;
    matlab << "%  plot(cell2mat(rfactor(frame)));" << std::endl;
    matlab << "%end" << std::endl;
    matlab << "%title('R-factor development', 'FontSize', f_size, 'FontName',f_type );" << std::endl;
    matlab << "%xlabel('Solver iteration', 'FontSize', f_size, 'FontName', f_type); ";
    matlab << "%ylabel('R-factor', 'FontSize', f_size, 'FontName', f_type);";
    matlab << "%print(gcf,'-depsc2','rfactors');" << std::endl;
    matlab << "%print(gcf,'-dpng','rfactors');" << std::endl;
    matlab << std::endl;
    matlab << std::endl;
    matlab.flush();
    matlab.close();

    logging << "Engine::write_matlab_profiling_data(): Done writing profile data..." << newline;

    return true;
  }

  bool Engine::write_matlab_contact_data(
                                  std::string const & filename
                                  , unsigned int const & frame_number
                                  )
  {
    typedef typename EngineData::T  T;

    util::Log        logging;

    std::string const newline = util::Log::newline();

    std::ofstream matlab;

    // create directories if necessary
    boost::filesystem::path contact_data_file( filename );

    if ( contact_data_file.has_parent_path() &&
        ! boost::filesystem::exists( contact_data_file.parent_path() ) &&
        ! boost::filesystem::create_directories( contact_data_file.parent_path() ) )
    {
      util::Log logging;

      logging << "Engine::write_matlab_contact_data(): Could not create directories for file '"
      << filename
      << "'"
      << util::Log::newline();
    }

    matlab.open(filename.c_str(),std::ios::out);

    if(! matlab.is_open())
    {
      logging << "Engine::write_matlab_contact_data(): error could not open file = " << filename.c_str() << util::Log::newline();

      return false;
    }

    std::vector<T> CX;
    std::vector<T> CY;
    std::vector<T> CZ;
    std::vector<T> NX;
    std::vector<T> NY;
    std::vector<T> NZ;
    std::vector<T> D;
    std::vector<T> AX;
    std::vector<T> AY;
    std::vector<T> AZ;
    std::vector<T> BX;
    std::vector<T> BY;
    std::vector<T> BZ;
    std::vector<T> SX;
    std::vector<T> SY;
    std::vector<T> SZ;
    std::vector<unsigned int> A;
    std::vector<unsigned int> B;

    for(unsigned int i=0u; i < m_data->m_contacts.size(); ++i)
    {
      T const cx = m_data->m_contacts[i].get_position()(0);
      T const cy = m_data->m_contacts[i].get_position()(1);
      T const cz = m_data->m_contacts[i].get_position()(2);
      T const nx = m_data->m_contacts[i].get_normal()(0);
      T const ny = m_data->m_contacts[i].get_normal()(1);
      T const nz = m_data->m_contacts[i].get_normal()(2);
      T const d = m_data->m_contacts[i].get_depth();
      T const ax = m_data->m_contacts[i].get_body_i()->get_position()(0);
      T const ay = m_data->m_contacts[i].get_body_i()->get_position()(1);
      T const az = m_data->m_contacts[i].get_body_i()->get_position()(2);
      T const bx = m_data->m_contacts[i].get_body_j()->get_position()(0);
      T const by = m_data->m_contacts[i].get_body_j()->get_position()(1);
      T const bz = m_data->m_contacts[i].get_body_j()->get_position()(2);
      T const sx = m_data->m_contacts[i].get_S()(0);
      T const sy = m_data->m_contacts[i].get_S()(1);
      T const sz = m_data->m_contacts[i].get_S()(2);


      unsigned int const idxA = m_data->m_contacts[i].get_body_i()->get_idx();
      unsigned int const idxB = m_data->m_contacts[i].get_body_j()->get_idx();

      CX.push_back(cx);
      CY.push_back(cy);
      CZ.push_back(cz);
      NX.push_back(nx);
      NY.push_back(ny);
      NZ.push_back(nz);
      D.push_back(d);
      AX.push_back(ax);
      AY.push_back(ay);
      AZ.push_back(az);
      BX.push_back(bx);
      BY.push_back(by);
      BZ.push_back(bz);
      A.push_back(idxA);
      B.push_back(idxB);
      SX.push_back(sx);
      SY.push_back(sy);
      SZ.push_back(sz);
    }

    matlab << "CX_" << frame_number << " = " << util::write_matlab_vector(CX) << ";" << std::endl;
    matlab << "CY_" << frame_number << " = " << util::write_matlab_vector(CY) << ";" << std::endl;
    matlab << "CZ_" << frame_number << " = " << util::write_matlab_vector(CZ) << ";" << std::endl;
    matlab << "NX_" << frame_number << " = " << util::write_matlab_vector(NX) << ";" << std::endl;
    matlab << "NY_" << frame_number << " = " << util::write_matlab_vector(NY) << ";" << std::endl;
    matlab << "NZ_" << frame_number << " = " << util::write_matlab_vector(NZ) << ";" << std::endl;
    matlab << "D_" << frame_number << " = " << util::write_matlab_vector(D) << ";" << std::endl;
    matlab << "AX_" << frame_number << " = " << util::write_matlab_vector(AX) << ";" << std::endl;
    matlab << "AY_" << frame_number << " = " << util::write_matlab_vector(AY) << ";" << std::endl;
    matlab << "AZ_" << frame_number << " = " << util::write_matlab_vector(AZ) << ";" << std::endl;
    matlab << "BX_" << frame_number << " = " << util::write_matlab_vector(BX) << ";" << std::endl;
    matlab << "BY_" << frame_number << " = " << util::write_matlab_vector(BY) << ";" << std::endl;
    matlab << "BZ_" << frame_number << " = " << util::write_matlab_vector(BZ) << ";" << std::endl;
    matlab << "A_" << frame_number << " = " << util::write_matlab_vector(A) << ";" << std::endl;
    matlab << "B_" << frame_number << " = " << util::write_matlab_vector(B) << ";" << std::endl;
    matlab << "SX_" << frame_number << " = " << util::write_matlab_vector(SX) << ";" << std::endl;
    matlab << "SY_" << frame_number << " = " << util::write_matlab_vector(SY) << ";" << std::endl;
    matlab << "SZ_" << frame_number << " = " << util::write_matlab_vector(SZ) << ";" << std::endl;

    matlab.flush();
    matlab.close();

    logging << "Engine::write_matlab_contact_data(): Done writing contact data..." << newline;

    return true;
  }


} // namespace prox
