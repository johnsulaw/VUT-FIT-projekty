<?php

namespace App\Modules\CoreModule\Presenters;

use Nette;

class DashboardPresenter extends Nette\Application\UI\Presenter
{
    /**
     * @inject
     * @var Nette\Database\Context
     */
    public $database;
    private $adminmodel;

	/*
	 * Database constructor
	 */
	public function __construct(Nette\Database\Context $database, \AdminModel $adminmodel)
	{
		$this->adminmodel = $adminmodel;
	}
        
	public function renderDefault() : void
    {
        $this->template->users = $this->database->table('user');
        $this->template->hotels = $this->database->table('hotel');
        $this->template->guest_reservations =
            $this->database->table('reservation')
                ->where('guest_mail', $this->template->user->getId());
        $this->template->reservations = $this->database->table('reservation');
        //$this->template->img = "../img/hotels/".$this->template->hotels->image;
    }
        
        public function getHotelName($hotel_id){
            return $this->database->table('hotel')->where('id', $hotel_id)->fetch()['name'];
        }

        public function getHotelImage($hotel_id){
            return $this->database->table('hotel')->where('id', $hotel_id)->fetch()['image'];
        }
        
        public function handleRemoveUser($usr){
            $this->database->table('reservation')->where('guest_mail', $usr)->delete();
            $this->database->table('receptions')->where('guest_mail', $usr)->delete();
            $this->database->table('user')->where('guest_mail', $usr)->delete();
            $this->database->table('guest')->where('mail_address', $usr)->delete();
        }
        
        public function handleRemoveHotel($hotel){
            $this->database->table('reservation')->where('hotel_id', $hotel)->delete();
            $this->database->table('room')->where('hotel_id', $hotel)->delete();
            $this->database->table('room_details')->where('hotel_id', $hotel)->delete();
            $this->database->table('receptions')->where('hotel_id', $hotel)->delete();
            $this->database->table('hotel')->where('id', $hotel)->delete();
        }
        
        public function handleAdd(){
            $this->adminmodel->addUser();
        }
        
        public function handleDeleteReservation($reservation_id){
            $this->database->table('reservation')
                    ->where('id', $reservation_id)
                    ->delete();
        }
        
        public function handleConfirmReservation($reservation_id){
            $this->database->query('UPDATE reservation SET', [
            'payed_off' => 1,
            ],
            'WHERE id = ?', $reservation_id);
        
        }
        
        public function getHotelId($guest_mail){
            $row = $this->database->table('receptions')
                    ->where('guest_mail', $guest_mail)
                    ->fetch();  
            
            return $row->hotel_id;
        }
}
